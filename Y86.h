// File Name:   Y86.h
// Author(s):   efb
// Description: Y86 constants 

#ifndef Y86_H
#define Y86_H

#include <iostream>
#include <cstdint>
#include <string>
#include "Memory.h"
#include "ProgRegisters.h"
#include "PipeStage.h"
#include "FetchStage.h"
#include "DecodeStage.h"
#include "ExecuteStage.h"
#include "WritebackStage.h"
#include "Forward.h"

class Y86 {
    // Program memory and registers.
    Memory memory;          
    ProgRegisters regs;
    // Pipe stages.
    PipeStage *stage[5];
    FetchStage fetchStage;
    DecodeStage decodeStage;
    ExecuteStage executeStage;
    MemoryStage memoryStage;
    WritebackStage writebackStage;
    // Forward object.
    Forward forward;
    // Count number of cycles.
    int cycles;
    // Whether each stage shoulf be stalled or bubbles.
    bool F_bubble;
    bool F_stall;
    bool D_stall;
    bool D_bubble;
    bool E_stall;
    bool E_bubble;
    bool M_stall;
    bool M_bubble;
    bool W_stall;
    bool W_bubble;
    // Private methods.
    void pipeControl();
	// Methods in Y86Loader.cpp.
    bool readFile (std::ifstream&);
    void storeData (uint64_t, uint8_t [10], int);
    int writeMemory	(std::string, uint64_t);
    void getLine (uint64_t *, uint64_t);
    // Methods in Y86trace.cpp.
	std::string getFlagsString ();
    // Public methods.
    public:
        Y86();
        void reset();
		void clock();
        uint64_t getStat();
        int getCycles() {return cycles;};
        // Methods in Y86Loader.cpp.
        bool load(char *);
		// Methods in Y86trace.cpp.
        void dumpMemory();			
		void dumpProgramRegisters();
		void dumpProcessorRegisters();
		void setTrace(bool, bool, bool, bool, bool);
        void trace(void);

        Memory& getMemory() {return memory;}  
        ProgRegisters& getProgRegisters() {return regs;}
};

#endif
