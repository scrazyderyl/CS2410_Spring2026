#include "components/functional_units/FPDivUnit.h"

FPDivUnit::FPDivUnit()
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED)
{
}

double FPDivUnit::calculateResult(const ReservationStation &rs)
{
    double v2 = rs.src2_value;

    if (v2 == 0)
    {
        return 0;
    }

    double v1 = rs.src1_value;

    return v1 / v2;
}
