// File Name:   Register.h
// Author(s):   efb
// Description: Y86 constants 

#ifndef REGISTER_H
#define REGISTER_H

template <class T> class Register {
    // Value in register before and after being clocked.
    T input;
    T state;
    // Public methods.
    public:
        Register() {state = 0; input = 0;}
        void reset() {state = 0;input = 0;}
		void reset(T val) {state = val; input = val;}
        void clock() {state = input;}
        void setInput(T next) {input = next;}
        T getInput() {return input;}
        T getState() {return state;}
};        

#endif
