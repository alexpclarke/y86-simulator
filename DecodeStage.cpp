// File Name:   DecodeStage.cpp
// Author(s):   Alex Clarke & Nicholas Noonan
// Description: Implementation of the decode stage. 

#include "Y86.h"
#include "DecodeStage.h"

// Reset the decode stage.
void DecodeStage::reset(ExecuteStage *_executeStage, ProgRegisters *_regs, 
Forward *_forward) {
    executeStage = _executeStage;
    regs = _regs;
    forward = _forward;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    rA.setInput(RNONE);
    rB.setInput(RNONE);
    valC.setInput(0);
    valP.setInput(0);
    stageStall = false;
    stageBubble = false;
}

// Set the values of the decode register.
void DecodeStage::updateDRegister(uint64_t f_stat, uint64_t f_icode,
uint64_t f_ifun, uint64_t f_rA, uint64_t f_rB, uint64_t f_valC,
uint64_t f_valP) {
    stat.setInput(f_stat);
    icode.setInput(f_icode);
    ifun.setInput(f_ifun);
    rA.setInput(f_rA);
    rB.setInput(f_rB);
    valC.setInput(f_valC);
    valP.setInput(f_valP);
}

// Clock phase 0.
void DecodeStage::clockP0() {
    // If stageStall is set, don't update the register.
    if (stageStall) {
        stageStall = false;
        return;
    }
    // If stageBubble is set, replace the isntruction with a bubble.
    if (stageBubble) {
        stageBubble = false;
        updateDRegister(SBUB, INOP, FNONE, RNONE, RNONE, 0, 0);
    }
    // Initial signals.
    stat.clock();
    D_stat = stat.getState();
    icode.clock();
    D_icode = icode.getState();
    ifun.clock();
    D_ifun = ifun.getState();
    rA.clock();
    D_rA = rA.getState();
    rB.clock();
    D_rB = rB.getState();
    valC.clock();
    D_valC = valC.getState();
    valP.clock();
    D_valP = valP.getState();
    // Calculated signals.
    d_dstE = getDstE();
    d_dstM = getDstM();
    d_srcA = getSrcA();
    d_srcB = getSrcB(); 
}

// Clock phase 1.
void DecodeStage::clockP1() {
    // Forwarded signals.
    e_dstE = forward->getedstE();
    e_valE = forward->getevalE();
    M_dstM = forward->getMdstM();
    m_valM = forward->getmvalM();
    M_dstE = forward->getMdstE();
    M_valE = forward->getMvalE();
    W_dstE = forward->getWdstE();
    W_valE = forward->getWvalE();
    W_dstM = forward->getWdstM();
    W_valM = forward->getWvalM();
    // Values from registers.
    d_rvalA = regs->getReg(d_srcA);
    d_rvalB = regs->getReg(d_srcB);
    // Calculated signals.
    d_valA = selectFwdA();
    d_valB = forwardB();
    // Pass signals to execute stage.
    executeStage->updateERegister(D_stat, D_icode, D_ifun, D_valC, d_valA,
    d_valB, d_dstE, d_dstM, d_srcA, d_srcB);
}


uint64_t DecodeStage::getSrcA() {
    switch (D_icode) {
        case IOPX:
        case IRRMOVQ:
        case IRMMOVQ:
        case IPUSHQ:
            return D_rA;
        case IPOPQ:
        case IRET:
            return RSP;
        default:
            return RNONE;
    }
}

uint64_t DecodeStage::getSrcB() {
    switch (D_icode) {
        case IOPX:
        case IRMMOVQ:
        case IMRMOVQ:
            return D_rB;
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            return RSP;
        default:
            return RNONE;
    }
}

uint64_t DecodeStage::getDstE() {
    switch (D_icode) { 
        case IIRMOVQ:
        case IOPX:
        case IRRMOVQ:
            return D_rB;
        case IPUSHQ:
        case IPOPQ:
        case ICALL:
        case IRET:
            return RSP;
        default: 
            return RNONE;
    }
}

uint64_t DecodeStage::getDstM() {
    switch (D_icode) {
        case IMRMOVQ:
        case IPOPQ:
            return D_rA;
        default:
            return RNONE;
    }
}

uint64_t DecodeStage::selectFwdA() {
    if (D_icode == ICALL) return D_valP;
    else if (D_icode == IJXX) return D_valP;
    else if (d_srcA == e_dstE) return e_valE;
    else if (d_srcA == M_dstM) return m_valM;
    else if (d_srcA == M_dstE) return M_valE;
    else if (d_srcA == W_dstM) return W_valM;
    else if (d_srcA == W_dstE) return W_valE;
    else return d_rvalA;
}

uint64_t DecodeStage::forwardB() {
    if (d_srcB == e_dstE) return e_valE;
    else if (d_srcB == M_dstM) return m_valM;
    else if (d_srcB == M_dstE) return M_valE;
    else if (d_srcB == W_dstM) return W_valM;
    else if (d_srcB == W_dstE) return W_valE;
    else return d_rvalB;
}
