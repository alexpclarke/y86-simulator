// Fine Name:   MemoryStage.cpp
// Author(s):   Alex Clarke & Nicholas Noonan
// Description: 

#include "Y86.h"
#include "MemoryStage.h"

// Reset the memory stage.
void MemoryStage::reset(WritebackStage *_writebackStage, Memory *_memory,
Forward *_forward) {
    writebackStage = _writebackStage;
    memory = _memory;
    forward = _forward;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    cnd.setInput(false);
    valE.setInput(0);
    valA.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
}

// Set the values of the decode register.
void MemoryStage::updateMRegister(uint64_t E_stat, uint64_t E_icode,
uint64_t E_ifun, bool e_cnd, uint64_t e_valE, uint64_t E_valA, uint64_t e_dstE,
uint64_t E_dstM) {
    stat.setInput(E_stat);
    icode.setInput(E_icode);
    ifun.setInput(E_ifun);
    cnd.setInput(e_cnd);
    valE.setInput(e_valE);
    valA.setInput(E_valA);
    dstE.setInput(e_dstE);
    dstM.setInput(E_dstM);
}

// Clock phase 0.
void MemoryStage::clockP0() {
    // If stageStall is set, don't update the register.
    if (stageStall) {
        stageStall = false;
        return;
    }
    // If stageBubble is set, replace the isntruction with a bubble.
    if (stageBubble) {
        stageBubble = false;
        updateMRegister(SBUB, INOP, FNONE, false, 0, 0, RNONE, RNONE);
    }
    // Initial signals.
    stat.clock();
    M_stat = stat.getState();
    icode.clock();
    M_icode = icode.getState();
    ifun.clock();
    M_ifun = ifun.getState();
    cnd.clock();
    M_Cnd = cnd.getState();
    valE.clock();
    M_valE = valE.getState();
    valA.clock();
    M_valA = valA.getState();
    dstE.clock();
    M_dstE = dstE.getState();
    dstM.clock();
    M_dstM = dstM.getState();
    // Calculated signals.
    m_addr = getAddr();
    mem_write = isWrite();
    mem_read = isRead();
    // Attempt to read from memory.
    m_valM = getValM();
    // Attempt to write to memory.
    if (mem_write) memory->putWord(m_addr, M_valA);
    // If there was an error reading or writing, set the exception.
    if (memory->isError()) {
        m_valM = 0;
        m_stat = SADR;
    } else {
        m_stat = M_stat;
    }
    // Pass signals to forward.
    forward->setmstat(m_stat);
    forward->setMdstM(M_dstM);
    forward->setmvalM(m_valM);
    forward->setMdstE(M_dstE);
    forward->setMvalE(M_valE);
    forward->setMicode(M_icode);
    forward->setMCnd(M_Cnd);
    forward->setMvalA(M_valA);
}

// Clock phase 1.
void MemoryStage::clockP1() {
    // Pass signals to writeback.
    writebackStage->updateWRegister(m_stat, M_icode, M_ifun, M_valE, m_valM,
    M_dstE, M_dstM);
}


uint64_t MemoryStage::getAddr() {
    switch (M_icode) {
        case IRMMOVQ:
        case IPUSHQ:
        case ICALL:
        case IMRMOVQ:
            return M_valE;
        case IPOPQ:
        case IRET:
            return M_valA;
        default:
            return 0;
    }
}

bool MemoryStage::isRead() {
    return M_icode == IMRMOVQ || M_icode == IPOPQ || M_icode == IRET;
}

bool MemoryStage::isWrite() {
    return M_icode == IRMMOVQ || M_icode == IPUSHQ || M_icode == ICALL;
}

uint64_t MemoryStage::getValM() {
    if (mem_read) {
        return memory->getWord(m_addr);
    }
    return 0;
}



