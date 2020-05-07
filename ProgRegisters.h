//File Name:    ProgRegisters.h
//Description:  Used to model program registers. 
//Author(s):    Alex Clarke & Nicholas Noonan

#ifndef PROGREGISTERS_H
#define PROGREGISTERS_H

#include <cstdint>
#include "Sim.h"
#include "Register.h"

class ProgRegisters {
    // Array of program registers and condition flags.
    Register<uint64_t> reg[NUM_REGISTERS];
	Register<uint64_t> CC;        
    // Public methods.
    public:
	    ProgRegisters();
        void clock();
        void setReg(uint64_t, uint64_t); 
        uint64_t getReg(uint64_t);
        void setCC(uint64_t, uint64_t);
        uint64_t getCC(uint64_t);
        void reset(); 
};

#endif
