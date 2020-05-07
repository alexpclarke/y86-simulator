// File Name:   ExecuteStage.cpp
// Author(s):   Alex Clarke & Nick Noonan
// Description: 

#include "Y86.h"
#include "ExecuteStage.h"
#include "ProgRegisters.h"

// Reset the execute stage.
void ExecuteStage::reset(MemoryStage *_memoryStage, ProgRegisters *_regs,
Forward *_forward) {
    memoryStage = _memoryStage;
    regs = _regs;
    forward = _forward;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valC.setInput(0);
    valA.setInput(0);
    valB.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
    srcA.setInput(RNONE);
    srcB.setInput(RNONE);
}

// Set the values of the execute register.
void ExecuteStage::updateERegister(uint64_t D_stat, uint64_t D_icode,
uint64_t D_ifun, uint64_t D_valC, uint64_t d_valA, uint64_t d_valB,
uint64_t d_dstE,uint64_t d_dstM, uint64_t d_srcA, uint64_t d_srcB) {
    stat.setInput(D_stat);
    icode.setInput(D_icode);
    ifun.setInput(D_ifun);
    valC.setInput(D_valC);
    valA.setInput(d_valA);
    valB.setInput(d_valB);
    dstE.setInput(d_dstE);
    dstM.setInput(d_dstM);
    srcA.setInput(d_srcA);
    srcB.setInput(d_srcB);
}

// Clock phase 0.
void ExecuteStage::clockP0() {
    // If stageStall is set, don't update the register.
    if (stageStall) {
        stageStall = false;
        return;
    }
    // If stageBubble is set, replace the instruction with a bubble.
    if (stageBubble) {
        stageBubble = false;
        updateERegister(SBUB, INOP, FNONE, 0, 0, 0, RNONE, RNONE, RNONE, RNONE);
    }
    // Initial signals.
    stat.clock();
    E_stat = stat.getState();
    icode.clock();
    E_icode = icode.getState();
    ifun.clock();
    E_ifun = ifun.getState();
    valC.clock();
    E_valC = valC.getState();
    valA.clock();
    E_valA = valA.getState();
    valB.clock();
    E_valB = valB.getState();
    dstE.clock();
    E_dstE = dstE.getState();
    dstM.clock();
    E_dstM = dstM.getState();
    srcA.clock();
    E_srcA = srcA.getState();
    srcB.clock();
    E_srcB = srcB.getState();
    // Calculated signals.
    e_aluA = getAluA();
    e_aluB = getAluB();
    e_alufun = getAluFunction();
    e_Cnd = getCnd();
    e_dstE = selectDstE();
    e_valE = getValE();
    // Pass signals to forward.
    forward->setedstE(e_dstE);
    forward->setevalE(e_valE);
}

// Clock phase 1.
void ExecuteStage::clockP1() {
    // Forwarded signals.
    m_stat = forward->getmstat();
    W_stat = forward->getWstat();
    // Determine whether there is an exeption in memory or writeback stage.
    bool m_exept = m_stat == SADR || m_stat == SINS || m_stat == SHLT;
    bool W_exept = W_stat == SADR || W_stat == SINS || W_stat == SHLT;
    // Determine whether we should set the condition flags and sets them.
    e_setCC = isSetCC() && !m_exept && !W_exept;
    if (e_setCC) setCC();
    // If an exeption is found, pass a bubble to memory stage, otherwise pass
    // the appropriate signals.
    if (m_exept || W_exept) {
        memoryStage->updateMRegister(SBUB, INOP, FNONE, 0, 0, 0, RNONE, RNONE);
    } else {
        memoryStage->updateMRegister(E_stat, E_icode, E_ifun, e_Cnd, e_valE,
        E_valA, e_dstE, E_dstM);
    }
}

// Determine the value of e_Cnd.
bool ExecuteStage::getCnd() {
    bool of = regs->getCC(OF);
    bool sf = regs->getCC(SF);
    bool zf = regs->getCC(ZF);
    switch (E_ifun) {
        case ALWAYS:
            return true;
        case LEQ:
            return (sf != of) || zf;
        case LT:
            return sf != of;
        case EQ:
            return zf;
        case NEQ:
            return !zf;
        case GTEQ:
            return sf == of;
        case GT:
            return !zf && (sf == of);
        default:
            return true;
    }
}   

// Determine the value of e_aluA.
uint64_t ExecuteStage::getAluA() {
    switch (E_icode) {
        case IOPX:
        case IRRMOVQ:
            return E_valA;
        case IIRMOVQ:
        case IRMMOVQ:
        case IMRMOVQ:
            return E_valC;
        case ICALL:
        case IPUSHQ:
            return -8;
        case IRET:
        case IPOPQ:
            return 8;
        default:
            return 0;
    }
}

// Determine the value of e_aluB.
uint64_t ExecuteStage::getAluB() {
    switch (E_icode) {
        case IRMMOVQ:
        case IMRMOVQ:
        case IOPX:
        case ICALL:
        case IPUSHQ:
        case IRET:
        case IPOPQ:
            return E_valB;
        case IIRMOVQ:
        case IRRMOVQ:
            return 0;
        default:
            return 0;
    }
}

// Determine whether we should set the CC or not.
bool ExecuteStage::isSetCC() {
    return E_icode == IOPX;
}

// Sets the condition flags based on the execution of the ALU.
void ExecuteStage::setCC() {
    // 
    bool zf = e_valE == 0;
    bool sf = e_valE >> 63 == 1;
    bool of = 0;
    bool isNeg_aluA = e_aluA >> 63;
    bool isNeg_aluB = e_aluB >> 63;
    bool isNeg_valE = e_valE >> 63;
    if (e_alufun == FADDQ) {
        if (!isNeg_aluA && !isNeg_aluB && isNeg_valE) {
            of = 1;
        } else if (isNeg_aluA && isNeg_aluB && !isNeg_valE) {
            of = 1;
        }
    } else if (e_alufun == FSUBQ) {
        if (!isNeg_aluA && isNeg_aluB && !isNeg_valE) {
            of = 1;
        } else if (isNeg_aluA && !isNeg_aluB && isNeg_valE) {
            of = 1;
        }
    }
    // Set the flags in the ProgRegisters.
    regs->setCC(ZF, zf);
    regs->setCC(SF, sf);
    regs->setCC(OF, of);
}

// Determine the value of e_alufun.
uint64_t ExecuteStage::getAluFunction() {
    return E_icode == IOPX ? E_ifun : FNONE;
}

// Determine the value for e_dstE.
uint64_t ExecuteStage::selectDstE() {
    return (E_icode == IRRMOVQ && !e_Cnd) ? RNONE : E_dstE;
}

// Acts as the ALU and determines the value of e_valE based on e_aluA, e_aluB
// and e_alufun.
uint64_t ExecuteStage::getValE() {
    switch (e_alufun) {
        case FSUBQ:
            return e_aluB - e_aluA;
        case FANDQ:
            return e_aluB & e_aluA;
        case FXORQ:
            return e_aluB ^ e_aluA;
        case FADDQ:
        default:
            return e_aluB + e_aluA;
    }
}
