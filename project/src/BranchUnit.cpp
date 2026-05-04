#include "BranchUnit.h"

BranchUnit::BranchUnit()
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED)
{
}

double BranchUnit::calculateResult(const ReservationStation &rs)
{
    double v1 = rs.src1_value;
    double v2 = rs.src2_value;
    bool taken = (v1 != v2);

    // Return 1 if branch is taken, 0 if not taken
    return taken ? 1.0 : 0.0;
}
