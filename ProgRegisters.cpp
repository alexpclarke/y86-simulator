// File Name:   ProgRegisters.cpp
// Author(s):   Alex Clarke & Nicholas Noonan 
// Description: Implements functions for the program register file.
    
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <assert.h>

#include "ProgRegisters.h"
#include "Tools.h"

// ProgRegisters constructor.
ProgRegisters::ProgRegisters() {
    reset();
}

// Calls clock method of each register and also the CC register.
void ProgRegisters::clock() {
    for (int i = 0; i < NUM_REGISTERS; i++) {   
        reg[i].clock();
    }
    CC.clock();
} 

// Sets the INPUT of the specified register.
void ProgRegisters::setReg(uint64_t regNum, uint64_t rval) {
	assert(regNum <= 15);
    if (regNum < NUM_REGISTERS) {
        reg[regNum].setInput(rval);
    }
}

// Returns STATE of specified register.
uint64_t ProgRegisters::getReg(uint64_t regNum) {
    assert(regNum <= 15);
    if (regNum == 15) {
        return 0;
    } else if (regNum <= NUM_REGISTERS) {
        return reg[regNum].getState();
    } else {
        return 0;
    }
}
       
// Sets the INPUT of the specified CC bit to the specified value (0 or 1).
void ProgRegisters::setCC(uint64_t bitNumber, uint64_t val) {
    assert(bitNumber <= 2 && val <= 1);
    uint64_t mask = 1;
    mask = mask << bitNumber;
    uint64_t ccVal = CC.getInput();
    if (val == 1) {
        ccVal = ccVal | mask;
    } else if (val == 0) {
        mask = ~mask;
        ccVal = ccVal & mask;
    }    
    CC.setInput(ccVal);
}

// Returns the STATE of the specified CC bit.
uint64_t ProgRegisters::getCC(uint64_t bitNumber) {
	assert(bitNumber <= 2);
    uint64_t ccVal = (uint64_t) CC.getState();
    ccVal = ccVal >> bitNumber;
    ccVal = ccVal & 1;
    return ccVal;
}
          
// Resets registers and CC flags.
void ProgRegisters::reset() {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        reg[i].reset();
    }
    setCC(OF, 0);
    setCC(SF, 0);
    setCC(ZF, 1); 
}
