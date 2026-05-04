#include "FPMultUnit.h"

FPMultUnit::FPMultUnit()
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED)
{
}

double FPMultUnit::calculateResult(const ReservationStation &rs)
{
    double v1 = rs.src1_value;
    double v2 = rs.src2_value;
    
    return v1 * v2;
}
