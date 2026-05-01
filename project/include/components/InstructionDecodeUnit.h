#ifndef COMPONENTS_INSTRUCTIONDECODEUNIT_H
#define COMPONENTS_INSTRUCTIONDECODEUNIT_H

#define NUM_INSTRUCTIONS 11

#include "types/DecodedInstruction.h"
#include "components/ExecutionUnit.h"

class InstructionDecodeUnit
{
public:
    // Array to look up methods to reserve decoded instructions with the right execution unit
    // Unfortunately for efficiency I'm using the lovely C function pointer syntax
    static bool (*executionUnitLookup[NUM_INSTRUCTIONS])(const DecodedInstruction &);

    InstructionDecodeUnit();

    // Register a executor unit for an instruction
    static void registerInstructionExecuter(uint8_t op, ExecutionUnit *executor);
};

#endif // COMPONENTS_INSTRUCTIONDECODEUNIT_H
