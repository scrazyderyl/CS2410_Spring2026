#ifndef COMPONENTS_INTEGERUNIT_H
#define COMPONENTS_INTEGERUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class IntegerUnit : public FunctionalUnit
{
public:
    static constexpr int LATENCY = 1;
    static constexpr bool PIPELINED = true;

    IntegerUnit(const std::vector<ReservationStation*> &rs, RegisterFileEntry *regFile);

private:
    virtual double calculateResult(DecodedInstruction &inst) override;
    RegisterFileEntry *registerFile;
};

#endif // COMPONENTS_INTEGERUNIT_H
