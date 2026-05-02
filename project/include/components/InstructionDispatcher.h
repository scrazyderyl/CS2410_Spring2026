#ifndef COMPONENTS_INSTRUCTIONDISPATCHER_H
#define COMPONENTS_INSTRUCTIONDISPATCHER_H

#define NUM_INSTRUCTIONS 11

#include <cstdint>
#include "types/DecodedInstruction.h"
#include "components/FunctionalUnit.h"
#include "components/InstructionDecodeUnit.h"

#include <vector>

class Simulator;

class InstructionDispatcher
{
public:
    // Lookup table to find functional unit for an instruction opcode
    FunctionalUnit *executors[NUM_INSTRUCTIONS];

    InstructionDispatcher(Simulator &sim);

    void registerInstructionExecuter(uint8_t op, FunctionalUnit *executor);

    // Dispatch up to NW instructions from instruction queue into RSs
    void dispatch();

private:
    Simulator &simulator;
};

#endif // COMPONENTS_INSTRUCTIONDISPATCHER_H
