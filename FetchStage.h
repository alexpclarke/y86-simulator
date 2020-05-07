// File Name:   FetcheStage.h
// Author(s):   Alex Clarke & Nicholas Noonan
// Description: Header file for fetch stage.

#ifndef FETCHSTAGE_H
#define FETCHSTAGE_H

#include "Sim.h"
#include "PipeStage.h"
#include "Register.h"
#include "DecodeStage.h"
#include "Forward.h"

class FetchStage : public PipeStage {
    // Register values.
    Register<uint64_t> predPC;
	// Pointers to next stage and the program memory.
    DecodeStage *decodeStage;
    Memory *memory;
    // Initial signals.
    uint64_t F_predPC;
    // Calculated signals.
    bool imem_error;
    bool instr_valid;
    bool need_regids;
    bool need_valC;
    uint64_t f_pc;
    uint64_t f_stat;
    uint64_t f_icode;
    uint64_t f_ifun;
    uint64_t f_rA;
    uint64_t f_rB;
    uint64_t f_valC;
    uint64_t f_valP;
    // Forwarded signals.
    uint64_t M_icode;
    uint64_t W_icode;
    uint64_t M_valA;
    uint64_t W_valM;
    bool M_Cnd;
    // Private methods.
    void predictPC();
    uint64_t selectPC();
    uint64_t getIcode();
    uint64_t getIfun();
    uint64_t getValC();
    uint64_t getValP();
    bool isInstrValid();
    bool isNeedRegids();
    bool isNeedValC();
    uint64_t getRA();
    uint64_t getRB();
    uint64_t getStat(); 
	// Public methods.
    public:
		void reset(DecodeStage *, Memory *, Forward *);
		void clockP0();
		void clockP1();
		void trace();
};

#endif
