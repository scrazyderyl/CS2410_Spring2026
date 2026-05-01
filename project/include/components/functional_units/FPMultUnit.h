#ifndef COMPONENTS_FPMULTUNIT_H
#define COMPONENTS_FPMULTUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class FPMultUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 2;
    static constexpr int LATENCY = 4;
    static constexpr bool PIPELINED = false;

    FPMultUnit(RegisterFileEntry *regFile);

private:
    RegisterFileEntry *registerFile;

    virtual double calculateResult(const DecodedInstruction &inst) override;
};

#endif // COMPONENTS_FPMULTUNIT_H
