// File Name:    Memory.h
// Author(s):    Alex Clarke & Nicholas Noonan
// Description:  Headers for the Memory object

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include "Sim.h"

class Memory {
	// Private member data.
    uint64_t mem[MEMORY_SIZE];
	bool memError; 
	// Private member functions.
	void store (uint64_t waddr, uint64_t val);
	uint64_t fetch (uint64_t waddr);
	// Public interface
    public:
        // Constructor.
        Memory();
        // Gets byte from spec'd byte address.
        unsigned char getByte(uint64_t byteAddress);
        // Writes byte to spec'd byte address.
        void putByte(uint64_t byteAddress, uint8_t value);
        // Gets word from spec'd byte address.
        uint64_t getWord(uint64_t byteAddress);
        // Writes word to spec'd byte address.
        void putWord(uint64_t byteAddress, uint64_t wordValue);
        // Clears memory to all zero, memError=false.
        void reset();
		// Returns true if memory error.
        bool isError() {return memError;}
};

#endif
