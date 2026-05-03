#include "components/functional_units/FPAddUnit.h"

FPAddUnit::FPAddUnit()
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED)
{
}

double FPAddUnit::calculateResult(const ReservationStation &rs)
{
    double v1 = rs.src1_value;
    double v2 = rs.src2_value;

    switch (rs.inst.op)
    {
    case 6: // fadd
        return v1 + v2;
    case 7: // fsub
        return v1 - v2;
    default:
        return 0;
    }
}
