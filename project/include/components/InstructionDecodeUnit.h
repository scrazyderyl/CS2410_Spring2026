#ifndef COMPONENTS_INSTRUCTIONDECODEUNIT_H
#define COMPONENTS_INSTRUCTIONDECODEUNIT_H

#include "types/DecodedInstruction.h"
#include "components/FunctionalUnit.h"
#include "types/ArchitecturalRegister.h"

class Simulator;

class InstructionDecodeUnit
{
public:
    InstructionDecodeUnit(Simulator &sim);

    void decode();

private:
    Simulator &simulator;

    int allocatePhysicalRegister();
    int resolveSourceRegister(const ArchitecturalRegister &reg);
    int renameDestinationRegister(const ArchitecturalRegister &reg);
    bool isPhysicalRegisterPending(uint8_t physReg) const;
};

#endif // COMPONENTS_INSTRUCTIONDECODEUNIT_H
