// File Name:   Forward.cpp
// Author(s):   Alex cCarke and Nicholas Noonan
// Description: The file through wich forwarded signals pass.

#include "Forward.h"
#include "Sim.h"

// Reset the forward signals.
void Forward::reset() {
    e_dstE = RNONE;
    e_valE = 0;
    M_dstM = RNONE;
    m_valM = 0;
    M_dstE = RNONE;
    M_valE = 0;
    W_dstE = RNONE;
    W_valE = 0;
    W_dstM = RNONE;
    W_valM = 0;
    m_stat = SAOK;
    W_stat = SAOK;
    M_icode = SAOK;
    M_Cnd = false;
    M_valA = 0;
    W_icode = 0xc;
}
