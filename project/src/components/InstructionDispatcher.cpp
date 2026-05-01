#include "components/InstructionDispatcher.h"

void InstructionDispatcher::registerInstructionExecuter(uint8_t op, FunctionalUnit *executor)
{
    executors[op] = executor;
}