// File Name:   MemoryStage.h
// Author(s):   Alex Clarke & Nicholas Noonan
// Description: Header file for memory stage.

#ifndef MEMORYSTAGE_H
#define MEMORYSTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "WritebackStage.h"
#include "Forward.h"

class MemoryStage : public PipeStage {
    // Register values.
    Register <bool> cnd;
    Register <uint64_t> valE;
    Register <uint64_t> valA;
    Register <uint64_t> dstE;
    Register <uint64_t> dstM;
    // Pointers to next stage and the program memory.
    WritebackStage *writebackStage;
    Memory *memory;
    // Initial signals.
    uint64_t M_icode;
    uint64_t M_ifun;
    uint64_t M_stat;
    uint64_t M_Cnd;
    uint64_t M_valA;
    uint64_t M_valE;
    uint64_t M_dstE;
    uint64_t M_dstM;
    // Calculated signals.
    uint64_t m_stat;
    uint64_t m_addr;
    bool mem_read;
    bool mem_write;
    uint64_t m_valM; 
    // Private methods.
    uint64_t getAddr();
    bool isRead();
    bool isWrite();
    uint64_t getValM();
    // Public methods.
    public:
        void reset(WritebackStage *, Memory *, Forward *);
        void updateMRegister(uint64_t, uint64_t, uint64_t, bool, uint64_t,
        uint64_t, uint64_t, uint64_t);
        void clockP0();
        void clockP1();
        void trace();
};

#endif
