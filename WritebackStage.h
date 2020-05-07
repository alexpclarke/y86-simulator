// File Name:   WritebackStage.h
// Author(s):   Alex Clarke & Nicholas Noonan
// Description: Header file for memory stage.

#ifndef WRITEBACKSTAGE_H
#define WRITEBACKSTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "Forward.h"

class WritebackStage : public PipeStage {
    // Register values.
    Register<uint64_t> valE;
    Register<uint64_t> valM;
    Register<uint64_t> dstE;
    Register<uint64_t> dstM;
    // Pointer to the program registers.
    ProgRegisters *regs;
    // Initial signals.
    uint64_t W_stat;
    uint64_t W_icode;
    uint64_t W_ifun;
    uint64_t W_valE;
    uint64_t W_valM;
    uint64_t W_dstE;
    uint64_t W_dstM;
    // Private methods.
    void writeRegisters();
    // Public methods.
    public:
        void reset(ProgRegisters *, Forward *);
        void updateWRegister(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t,
        uint64_t, uint64_t);
        void clockP0();
        void clockP1();
        void trace();
};

#endif
