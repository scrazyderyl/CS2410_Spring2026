#ifndef COMPONENTS_FPDIVUNIT_H
#define COMPONENTS_FPDIVUNIT_H

#include "FunctionalUnit.h"
#include "DecodedInstruction.h"

#include <vector>

class FPDivUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 1;
    static constexpr int LATENCY = 6;
    static constexpr bool PIPELINED = false;

    FPDivUnit();

private:
    virtual double calculateResult(const ReservationStation &rs) override;
};

#endif // COMPONENTS_FPDIVUNIT_H
