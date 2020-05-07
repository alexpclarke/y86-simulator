//file name: WritebackStage.cpp
//authors: alex clark and nicholas noonan
//
#include "Y86.h"
#include "WritebackStage.h"

// Reset the writeback stage.
void WritebackStage::reset(ProgRegisters *_regs, Forward *_forward) {
    regs = _regs;
    forward = _forward;
    stat.setInput(SBUB);
    icode.setInput(INOP);
    ifun.setInput(FNONE);
    valE.setInput(0);
    valM.setInput(0);
    dstE.setInput(RNONE);
    dstM.setInput(RNONE);
}


//Set the values of the writeback register.
void WritebackStage::updateWRegister(uint64_t m_stat, uint64_t M_icode,
uint64_t M_ifun, uint64_t M_valE, uint64_t m_valM, uint64_t M_dstE,
uint64_t M_dstM) {
    stat.setInput(m_stat);
    icode.setInput(M_icode);
    ifun.setInput(M_ifun);
    valE.setInput(M_valE);
    valM.setInput(m_valM);
    dstE.setInput(M_dstE);
    dstM.setInput(M_dstM);
}

// Clock phase 0.
void WritebackStage::clockP0() {
    // If stageStall is set, don't update the register.
    if (stageStall) {
        stageStall = false;
        return;
    }
    // If stageBubble is set, replace the isntruction with a bubble.
    if (stageBubble) {
        stageBubble = false;
        updateWRegister(SBUB, INOP, FNONE, 0, 0, RNONE, RNONE);
    }
    // Initial signals.
    stat.clock();
    W_stat = stat.getState();
    icode.clock();
    W_icode = icode.getState();
    ifun.clock();
    W_ifun = ifun.getState();
    valE.clock();
    W_valE = valE.getState();
    valM.clock();
    W_valM = valM.getState();
    dstE.clock();
    W_dstE = dstE.getState();
    dstM.clock();
    W_dstM = dstM.getState();
    // Pass signals to forward.
    forward->setWdstE(W_dstE);
    forward->setWvalE(W_valE);
    forward->setWdstM(W_dstM);
    forward->setWvalM(W_valM);
    forward->setWicode(W_icode);
}

// Clock phase 1
void WritebackStage::clockP1() {
    writeRegisters();
}

void WritebackStage::writeRegisters() {
    if (W_dstE != RNONE) regs->setReg(W_dstE, W_valE);
    if (W_dstM != RNONE) regs->setReg(W_dstM, W_valM);
}
