#ifndef COMPONENTS_INTEGERUNIT_H
#define COMPONENTS_INTEGERUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"

#include <vector>

class IntegerUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 4;
    static constexpr int LATENCY = 1;
    static constexpr bool PIPELINED = true;

    IntegerUnit();

private:
    virtual double calculateResult(const ReservationStation &rs) override;
};

#endif // COMPONENTS_INTEGERUNIT_H
