// File Name:   ExecuteStage.h
// Author(s):   Alex Clarke & Nicholas Noonan
// Description: Header for the execute stage.

#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "MemoryStage.h"
#include "Register.h"
#include "ProgRegisters.h"
#include "Forward.h"

class ExecuteStage : public PipeStage {
    // Register values. 
    Register<uint64_t> valC;
    Register<uint64_t> valA;
    Register<uint64_t> valB;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;
    Register<uint64_t> srcA;
    Register<uint64_t> srcB;
    // Pointers to next stage and the program registers.
    MemoryStage *memoryStage;
    ProgRegisters *regs;
    // Initial signals.
    uint64_t E_stat;
    uint64_t E_icode;
    uint64_t E_ifun;
    uint64_t E_valC;
    uint64_t E_valA;
    uint64_t E_valB;
    uint64_t E_dstE;
    uint64_t E_dstM;
    uint64_t E_srcA;
    uint64_t E_srcB;
    // Calculated signals.
    uint64_t e_dstE;
    uint64_t e_aluA;
    uint64_t e_aluB;
    bool e_setCC;
    uint64_t e_alufun;
    bool e_Cnd;
    uint64_t e_valE;
    // Forwarded signals.
    uint64_t m_stat;
    uint64_t W_stat;
    // Private methods. 
    uint64_t getAluA();
    uint64_t getAluB();
    bool isSetCC();
    void setCC();
    uint64_t getAluFunction();
    uint64_t selectDstE();
    uint64_t getValE();
    // Public methods.
    public:
        void reset(MemoryStage *, ProgRegisters *, Forward *);
        void updateERegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
        uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
        void clockP0();
        void clockP1();
        bool getCnd();
        uint64_t getDstM() {return dstM.getState();}
        void trace();
};

#endif
