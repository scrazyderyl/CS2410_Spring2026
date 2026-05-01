#include "components/functional_units/FPDivUnit.h"

FPDivUnit::FPDivUnit(const std::vector<ReservationStation *> &rs, RegisterFileEntry *regFile)
    : FunctionalUnit(rs, PIPELINED, LATENCY), registerFile(regFile)
{
}

double FPDivUnit::calculateResult(DecodedInstruction &inst)
{
    double v2 = registerFile[inst.src2].value;

    if (v2 == 0)
    {
        return 0;
    }

    double v1 = registerFile[inst.src1].value;

    return v1 / v2;
}
