#ifndef COMPONENTS_FPADUNIT_H
#define COMPONENTS_FPADUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"

#include <vector>

class FPAddUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 3;
    static constexpr int LATENCY = 3;
    static constexpr bool PIPELINED = true;

    FPAddUnit();

private:
    virtual double calculateResult(const ReservationStation &rs) override;
};

#endif // COMPONENTS_FPADUNIT_H
