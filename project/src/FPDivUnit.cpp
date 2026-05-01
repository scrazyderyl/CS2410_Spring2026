#include "components/functional_units/FPDivUnit.h"

FPDivUnit::FPDivUnit(RegisterFileEntry *regFile)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), registerFile(regFile)
{
}

double FPDivUnit::calculateResult(const DecodedInstruction &inst)
{
    double v2 = registerFile[inst.src2].value;

    if (v2 == 0)
    {
        return 0;
    }

    double v1 = registerFile[inst.src1].value;

    return v1 / v2;
}
