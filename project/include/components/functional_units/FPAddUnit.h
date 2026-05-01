#ifndef COMPONENTS_FPADUNIT_H
#define COMPONENTS_FPADUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class FPAddUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 3;
    static constexpr int LATENCY = 3;
    static constexpr bool PIPELINED = true;

    FPAddUnit(RegisterFileEntry *regFile);

private:
    RegisterFileEntry *registerFile;

    virtual double calculateResult(const DecodedInstruction &inst) override;
};

#endif // COMPONENTS_FPADUNIT_H
