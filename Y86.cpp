//File Name:    Y86.cpp
//Description:  Implementation of Y86 class
//Author(s):    Alex Clarke & Nicholas Noonan

#include <cstdint>
#include "Y86.h"

//Constructor
Y86::Y86() {
    memory.reset();
}

//resets Y86 object to initial state.
void Y86::reset() {
    stage[0] = &fetchStage;
    stage[1] = &decodeStage;
    stage[2] = &executeStage;
    stage[3] = &memoryStage;
    stage[4] = &writebackStage;

    fetchStage.reset(&decodeStage, &memory, &forward);
    decodeStage.reset(&executeStage, &regs, &forward);
    executeStage.reset(&memoryStage, &regs, &forward);
    memoryStage.reset(&writebackStage, &memory, &forward);
    writebackStage.reset(&regs, &forward);

    cycles = 0;
    forward.reset();
    memory.reset();
    regs.reset();
    regs.clock();
}

// Gets the status of the last completed instruction.
uint64_t Y86::getStat() {
    uint64_t stat = writebackStage.getStat();
    if (stat == SAOK || stat == SBUB) {
        return 0;
    } else {
        return stat;
    }
}

void Y86::clock() {
    regs.clock();
    for (int i = 0; i < 5; i++) {
        stage[i]->clockP0();
    }
    for (int i = 0; i < 5; i++) {
        stage[i]->clockP1();
    }
    pipeControl();
}

void Y86::pipeControl(){
    uint64_t E_icode = executeStage.getIcode();
    uint64_t E_dstM = executeStage.getDstM();
    uint64_t D_icode = decodeStage.getIcode();
    uint64_t M_icode = memoryStage.getIcode();
    uint64_t d_srcA = decodeStage.getSrcA();
    uint64_t d_srcB = decodeStage.getSrcB();
    uint64_t m_stat = memoryStage.getStat();
    uint64_t W_stat = writebackStage.getStat();
    bool e_Cnd = executeStage.getCnd();
    //fetchstagecontrol
    F_bubble = 0;
    F_stall = ((E_icode == IMRMOVQ || E_icode == IPOPQ) && 
        (E_dstM == d_srcA || E_dstM == d_srcB)) || 
        (D_icode == IRET || E_icode == IRET || M_icode == IRET);
    
    //decodestagecontrol
    D_stall = (E_icode == IMRMOVQ || E_icode == IPOPQ) &&
        (E_dstM == d_srcA || E_dstM == d_srcB);

    D_bubble = (E_icode == IJXX && !e_Cnd) || 
        (!((E_icode == IMRMOVQ || E_icode == IPOPQ) && 
        (E_dstM == d_srcA || E_dstM == d_srcB)) && 
        (D_icode == IRET || E_icode == IRET || M_icode == IRET));

    //executestagecontrol
    E_stall = 0;
    E_bubble = ((E_icode == IJXX && !e_Cnd) || 
        ((E_icode == IMRMOVQ || E_icode == IPOPQ) && 
        (E_dstM == d_srcA || E_dstM == d_srcB)));
    
    //memortstagecontrol
    M_stall = 0;
    M_bubble = ((m_stat == SADR || m_stat == SINS || m_stat == SHLT) || 
        (W_stat == SADR || W_stat == SINS || W_stat == SHLT));
    
    W_stall = (W_stat == SADR || W_stat == SINS || W_stat == SHLT);
    W_bubble = 0;

    fetchStage.setBubble(F_bubble);
    fetchStage.setStall(F_stall);

    decodeStage.setBubble(D_bubble);
    decodeStage.setStall(D_stall);

    executeStage.setBubble(E_bubble);
    executeStage.setStall(E_stall);

    memoryStage.setBubble(M_bubble);
    memoryStage.setStall(M_stall);

    writebackStage.setBubble(W_bubble);
    writebackStage.setStall(W_stall);
}
