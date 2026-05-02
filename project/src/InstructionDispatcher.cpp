#include "components/InstructionDispatcher.h"
#include "Simulator.h"

InstructionDispatcher::InstructionDispatcher(Simulator &sim) : simulator(sim) {}

void InstructionDispatcher::registerInstructionExecuter(uint8_t op, FunctionalUnit *executor)
{
    executors[op] = executor;
}

void InstructionDispatcher::dispatch()
{
    
}