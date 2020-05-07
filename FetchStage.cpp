#include "Y86.h"
#include "FetchStage.h"

// Reset the fetch stage.
void FetchStage::reset(DecodeStage *_decodeStage, Memory *_memory,
Forward *_forward) {
    forward = _forward;
    decodeStage = _decodeStage;
    memory = _memory;
	predPC.reset();
}

// Clock phase 0.
void FetchStage::clockP0() {
	// If stageStall is set, don't update the register.
    if (stageStall) {
        stageStall = false;
        return;
    }
    // Initial signals.
    predPC.clock();
    F_predPC = predPC.getState();
}

// Clock phase 1.
void FetchStage::clockP1() {
    // Forwarded signals.
    M_icode = forward->getMicode();
    M_Cnd = forward->getMCnd();
    M_valA = forward->getMvalA();
    W_icode = forward->getWicode();
    W_valM = forward->getWvalM();
    // Calculated signals.
    f_pc = selectPC();
    f_icode = getIcode();
    instr_valid = isInstrValid();
    f_ifun = getIfun();
    f_stat = getStat(); 
    need_regids = isNeedRegids();
    f_rA = getRA();
    f_rB = getRB();
    need_valC = isNeedValC();
    f_valC = getValC();
    f_valP = getValP();
    // Predict the memory location of the next instruction.
    predictPC();
    // Pass signals to decode stage.
	decodeStage->updateDRegister(f_stat, f_icode, f_ifun, f_rA, f_rB, f_valC,
     f_valP);
}

bool FetchStage::isNeedRegids() {
    switch (f_icode) {
        case IRRMOVQ:
        case IOPX:
        case IPUSHQ:
        case IPOPQ:
        case IIRMOVQ:
        case IRMMOVQ:
        case IMRMOVQ:
            return true;
        default:
            return false;
    }
}

bool FetchStage::isNeedValC() {
    switch (f_icode) {
        case IIRMOVQ:
        case IRMMOVQ:
        case IMRMOVQ:
        case IJXX:
        case ICALL:
            return true;
        default:
            return false;
    }
}

bool FetchStage::isInstrValid() {
    switch (f_icode) {
        case INOP:
        case IHALT:
        case IRRMOVQ:
        case IIRMOVQ:
        case IRMMOVQ:
        case IMRMOVQ:
        case IOPX:
        case IJXX:
        case ICALL:
        case IRET:
        case IPUSHQ:
        case IPOPQ:
            return true;
        default:
            return false;
    }
}

uint64_t FetchStage::getIcode() {
    uint64_t inst = memory->getByte(f_pc);
    return (inst >> 4) & 0xF;
}

uint64_t FetchStage::getIfun() {
    uint64_t inst = memory->getByte(f_pc);
    return inst & 0xF;
}

uint64_t FetchStage::getStat() {
    if (f_icode == IHALT) return SHLT;
    else if (!instr_valid) return SINS;
    else return SAOK;
}

void FetchStage::predictPC() {
	switch (f_icode) {
        case IJXX:
        case ICALL:
            predPC.setInput(f_valC);
            break;
        default:
            predPC.setInput(f_valP);
            break;
    }
}

uint64_t FetchStage::getValC() {
    if (need_valC) {
        if (need_regids) return memory->getWord(f_pc + 2);
        else return memory->getWord(f_pc + 1);
    } else {
        return 0;
    }
}

uint64_t FetchStage::getRA() {
    if (need_regids) {
        return (memory->getByte(f_pc + 1) >> 4) & 0xF;
    } else {
        return RNONE;
    }
}

uint64_t FetchStage::getRB() {
    if (need_regids) {
        return memory->getByte(f_pc + 1) & 0xF;
    } else {
        return RNONE;
    }
}

uint64_t FetchStage::getValP() {
    if (need_regids && need_valC) return f_pc + 10;
    else if (need_regids && !need_valC) return f_pc + 2;
    else if (!need_regids && need_valC) return f_pc + 9;
    else return f_pc + 1;
}

uint64_t FetchStage::selectPC() {
	if (M_icode == IJXX && !M_Cnd) return M_valA;
    else if (W_icode == IRET) return W_valM;
    else return F_predPC;
}
