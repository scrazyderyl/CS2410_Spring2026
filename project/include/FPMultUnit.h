#ifndef COMPONENTS_FPMULTUNIT_H
#define COMPONENTS_FPMULTUNIT_H

#include "FunctionalUnit.h"
#include "DecodedInstruction.h"

#include <vector>

class FPMultUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 2;
    static constexpr int LATENCY = 4;
    static constexpr bool PIPELINED = false;

    FPMultUnit();

private:
    virtual double calculateResult(const ReservationStation &rs) override;
};

#endif // COMPONENTS_FPMULTUNIT_H
