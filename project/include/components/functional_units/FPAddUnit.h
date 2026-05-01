#ifndef COMPONENTS_FPADUNIT_H
#define COMPONENTS_FPADUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class FPAddUnit : public FunctionalUnit
{
public:
    static constexpr int LATENCY = 3;
    static constexpr bool PIPELINED = true;

    FPAddUnit(const std::vector<ReservationStation *> &rs, RegisterFileEntry *regFile);

private:
    RegisterFileEntry *registerFile;

    virtual double calculateResult(DecodedInstruction &inst) override;
};

#endif // COMPONENTS_FPADUNIT_H
