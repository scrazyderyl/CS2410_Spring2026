#ifndef COMPONENTS_INSTRUCTIONDECODEUNIT_H
#define COMPONENTS_INSTRUCTIONDECODEUNIT_H

#include "types/DecodedInstruction.h"
#include "components/FunctionalUnit.h"

class Simulator;

class InstructionDecodeUnit
{
public:
    InstructionDecodeUnit(Simulator &sim);

    void decode();

private:
    Simulator &simulator;
};

#endif // COMPONENTS_INSTRUCTIONDECODEUNIT_H
