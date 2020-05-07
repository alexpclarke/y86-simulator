// File Name:   Forward.h
// Author(s):   Alex Clark & Nicholas Noonan
// Description: Header file for forward.

#ifndef FORWARD_H
#define FORWARD_H

#include <stdint.h>

class Forward {
    // Forwarded signals.
    uint64_t e_dstE;
    uint64_t e_valE;
    uint64_t M_dstM;
    uint64_t m_valM;
    uint64_t M_dstE;
    uint64_t M_valE;
    uint64_t W_dstE;
    uint64_t W_valE;
    uint64_t W_dstM;
    uint64_t W_valM;
    uint64_t m_stat;
    uint64_t W_stat;
    uint64_t M_icode;
    bool M_Cnd;
    uint64_t M_valA;
    uint64_t W_icode;
    // Public methods.
    public:
        void reset();
        // Getters and setters for every signal.
        uint64_t getedstE() {return e_dstE;}
        void setedstE(uint64_t _e_dstE) {e_dstE = _e_dstE;}
        uint64_t getevalE() {return e_valE;}
        void setevalE(uint64_t _e_valE) {e_valE = _e_valE;}
        uint64_t getMdstM() {return M_dstM;}
        void setMdstM(uint64_t _M_dstM) {M_dstM = _M_dstM;}
        uint64_t getmvalM() {return m_valM;}
        void setmvalM(uint64_t _m_valM) {m_valM = _m_valM;}
        uint64_t getMdstE() {return M_dstE;}
        void setMdstE(uint64_t _M_dstE) {M_dstE = _M_dstE;}
        uint64_t getMvalE() {return M_valE;}
        void setMvalE(uint64_t _M_valE) {M_valE = _M_valE;}
        uint64_t getWdstE() {return W_dstE;}
        void setWdstE(uint64_t _W_dstE) {W_dstE = _W_dstE;}
        uint64_t getWvalE() {return W_valE;}
        void setWvalE(uint64_t _W_valE) {W_valE = _W_valE;}
        uint64_t getWdstM() {return W_dstM;}
        void setWdstM(uint64_t _W_dstM) {W_dstM = _W_dstM;}
        uint64_t getWvalM() {return W_valM;}
        void setWvalM(uint64_t _W_valM) {W_valM = _W_valM;}
        uint64_t getmstat() {return m_stat;}
        void setmstat(uint64_t _m_stat) {m_stat = _m_stat;}
        uint64_t getWstat() {return W_stat;}
        void setWstat(uint64_t _W_stat) {W_stat = _W_stat;}
        uint64_t getMicode() {return M_icode;}
        void setMicode(uint64_t _M_icode) {M_icode = _M_icode;}
        bool getMCnd() {return M_Cnd;}
        void setMCnd(bool MCnd) {M_Cnd = MCnd;}
        uint64_t getMvalA() {return M_valA;}
        void setMvalA(uint64_t MvalA) {M_valA = MvalA;}
        uint64_t getWicode() {return W_icode;}
        void setWicode(uint64_t Wicode) {W_icode = Wicode;}  
};
#endif
