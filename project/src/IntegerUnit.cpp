#include "components/functional_units/IntegerUnit.h"

IntegerUnit::IntegerUnit()
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED)
{
}

double IntegerUnit::calculateResult(const ReservationStation &rs)
{
    double v1 = rs.src1_value;
    double v2 = rs.src2_value;

    switch (rs.inst->op)
    {
    case 3: // add
        return v1 + v2;
    case 4: // addi
        return v1 + rs.inst->imm;
    case 5: // slt
        return v1 < v2 ? 1 : 0;
    default:
        return 0;
    }
}
