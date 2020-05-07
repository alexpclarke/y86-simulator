CXX=g++
CXXFLAGS = -Wall -g -std=c++11

# Put object files in next line
OBJS = Memory.o ProgRegisters.o Sim.o Tools.o Y86Loader.o Y86trace.o Y86.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o Forward.o 

# The OBJTEST macro is same as 'OBJS' above except replace Sim.o with testSim.o 
OBJTEST = Memory.o ProgRegisters.o testSim.o Y86.o Tools.o 

#complete the following targets with appropriate dependencies and commands
yess:	$(OBJS)
	$(CXX) $(OBJS) -o yess
ytest:  $(OBJTEST)
	$(CXX) $(OBJTEST) -o ytest

#your object file targets below here...

Y86Loader.o: Y86Loader.cpp Y86.h Memory.h

Memory.o: Memory.cpp Memory.h Sim.h Tools.h

ProgRegisters.o: ProgRegisters.cpp ProgRegisters.h Tools.h Sim.h Register.h

Sim.o: Sim.cpp Sim.h Y86.h Memory.h ProgRegisters.h

Y86.o: Y86.cpp Y86.h Y86trace.cpp Memory.h ProgRegisters.h Y86Loader.cpp PipeStage.h FetchStage.h DecodeStage.h ExecuteStage.h WritebackStage.h

Tools.o: Tools.h

Y86trace.o: Y86trace.cpp Sim.h Y86.h PipeStage.h FetchStage.h DecodeStage.h ExecuteStage.h MemoryStage.h WritebackStage.h

FetchStage.o: FetchStage.cpp FetchStage.h Y86.h Sim.h PipeStage.h Register.h DecodeStage.h

DecodeStage.o: DecodeStage.cpp DecodeStage.h Y86.h Sim.h PipeStage.h Register.h ExecuteStage.h

ExecuteStage.o: ExecuteStage.cpp ExecuteStage.h Y86.h Sim.h PipeStage.h MemoryStage.h Register.h

MemoryStage.o: MemoryStage.cpp MemoryStage.h Y86.h Sim.h PipeStage.h Register.h WritebackStage.h

WritebackStage.o: WritebackStage.cpp WritebackStage.h Y86.h Sim.h PipeStage.h Register.h

Forward.o: Forward.cpp Forward.h Sim.h

# clean target
# Remeber, you don't want to delete your testSim.o file
# The following target preserves testSim.o
clean:
	rm -f *.o yess
	


###THIS IS TEST STUFF
buildTest: test.o
	$(CXX) test.o -o runTest
	runTest
test.o: test.cpp

