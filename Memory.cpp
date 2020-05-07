//File Name:    Memory.cpp
//Author(s):    Alex Clarke & Nicholas Noonan
//Description:  Implementation of the Memory class.
  
#include <iostream>
#include <iomanip>
#include <cstdint>

#include "Memory.h"
#include "Tools.h"

// Constructor
// Store 64-bit value to a word address in memory.
// Set memError if address out of bounds.
Memory::Memory() {
    reset();
}

// Store 64-bit value to a word address in memory.
// Set memError if address out of bounds.
void Memory::store(uint64_t waddr, uint64_t val) { 
   if (waddr < MEMORY_SIZE) {
       mem[waddr] = val;
   } else {
       memError = true;
   }
}

// Fetch a 64-bit value from a word address in memory.
// Set memError if address out of bounds.
uint64_t Memory::fetch(uint64_t waddr) {
    if (waddr < MEMORY_SIZE) {
        return mem[waddr];
    } else {
        memError = true;
        return 0;
    }
}

// Returns a byte from the specified byte address of Y86 memory array. 
// If address is out of range the error status is set.
unsigned char Memory::getByte(uint64_t byteAddress) {
    int waddr = byteAddress / 8;
    int baddr = byteAddress % 8;
    if (waddr < MEMORY_SIZE) {
        uint64_t word = Memory::fetch(waddr);
        return Tools::getByteNumber(baddr, word);
    } else {
        memError = true;
        return '\0';
    }
}

// Write a single byte into the Y86 simulated memory at the byte address
// specified by 'address'.
// If address is out of range the error status is set.                 
void Memory::putByte(uint64_t byteAddress, uint8_t value) {
    int waddr = byteAddress / 8;
    int baddr = byteAddress % 8;
    
    if (waddr < MEMORY_SIZE) {
        uint64_t word = Memory::fetch(waddr);
        word = Tools::putByteNumber(baddr, value, word);
        Memory::store(waddr, word);
    } else {
        memError = true;
    }
}

// Returns word starting from the specified byte address.
// No part of the word must lie outside memory range.
uint64_t Memory::getWord(uint64_t byteAddress) {
	if (byteAddress <= (8 * (MEMORY_SIZE - 1))) {
        uint64_t word = 0;
        for (int b = 0; b < 8; b++) {
            uint8_t byte = Memory::getByte(byteAddress + b);
            word = Tools::putByteNumber(b, byte, word);
        }
        return word;
    } else {
        memError = true;
        return 0;
    }
}

// Writes a word to memory starting at the specified byte address.
// No part of the word must lie outside memory range. 
// If there is a memory error (out of range) the memory should not be changed.
void Memory::putWord(uint64_t byteAddress, uint64_t wordValue) {
	if (byteAddress <= (8 * (MEMORY_SIZE - 1))) {
        for (int b = 0; b < 8; b++) {
            uint8_t byte = Tools::getByteNumber(b, wordValue);
            Memory::putByte(byteAddress + b, byte);
        }
    } else {
        memError = true;
    }
}

// Clears memory to all zero. Clears error status.
void Memory::reset() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        mem[i] = 0;
    }
    memError = false;
}
