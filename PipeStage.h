#ifndef PIPESTAGE_H
#define PIPESTAGE_H

#include "Sim.h"
#include "Register.h"
#include "Forward.h"

class Y86;

class PipeStage {
    protected:
        Register<uint64_t> stat;
        Register<uint64_t> icode;
		Register<uint64_t> ifun;
		bool traceEnabled;
        Forward *forward;
        bool stageStall;
        bool stageBubble;       
    public:      
		void setTrace(bool enable) {traceEnabled = enable;}
		uint64_t getStat() {return stat.getState();}
        uint64_t getIcode() {return icode.getState();}
        void setStall(bool _stageStall) {stageStall = _stageStall;}
        void setBubble(bool _stageBubble) {stageBubble = _stageBubble;}

        // Pure virtual--must be implemented in subclasses 
        virtual void clockP0() = 0;
        virtual void clockP1() = 0;
		virtual void trace() = 0;
};

#endif
