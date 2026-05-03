#ifndef COMPONENTS_BRANCHUNIT_H
#define COMPONENTS_BRANCHUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"

#include <vector>

class BranchUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 2;
    static constexpr int LATENCY = 1;
    static constexpr bool PIPELINED = true;

    BranchUnit();

private:
    virtual double calculateResult(const ReservationStation &rs) override;
};

#endif // COMPONENTS_BRANCHUNIT_H
