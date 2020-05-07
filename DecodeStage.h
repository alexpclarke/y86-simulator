// File Name:   DecodeStage.h
// Author(s):   Alex Clarke & Nicholas Noonan
// Description: Header file for decode stage.

#ifndef DECODESTAGE_H
#define DECODESTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "ExecuteStage.h"
#include "Forward.h"

class DecodeStage : public PipeStage {
    // Register values.
    Register <uint64_t> rA;
    Register <uint64_t> rB;
    Register <uint64_t> valC;
    Register <uint64_t> valP;
    // Pointers to next stage and the program memory.
    ExecuteStage *executeStage;
    ProgRegisters *regs;
    // Initial signals.
    uint64_t D_stat;
    uint64_t D_icode;
    uint64_t D_ifun;
    uint64_t D_rA;
    uint64_t D_rB;
    uint64_t D_valC;
    uint64_t D_valP;
    // Calculated signals.
    uint64_t d_dstE;
    uint64_t d_dstM;
    uint64_t d_srcA;
    uint64_t d_srcB;
    uint64_t d_rvalA;
    uint64_t d_rvalB;
    uint64_t d_valA;
    uint64_t d_valB;
    // Forwarded signals.
    uint64_t e_dstE;
    uint64_t e_valE;
    uint64_t M_dstM;
    uint64_t m_valM;
    uint64_t M_dstE;
    uint64_t M_valE;    
    uint64_t W_dstE;
    uint64_t W_valE;
    uint64_t W_dstM;
    uint64_t W_valM;
    // Private methods.
    uint64_t getDstE();
    uint64_t getDstM();
    uint64_t selectFwdA();
    uint64_t forwardB();
    // Public methods.
    public:
        void reset(ExecuteStage *, ProgRegisters *, Forward *);
        void updateDRegister(uint64_t, uint64_t,  uint64_t, uint64_t, uint64_t,
        uint64_t, uint64_t);
        void clockP0();
        void clockP1();
        uint64_t getSrcA();
        uint64_t getSrcB();
        void trace();
};

#endif
