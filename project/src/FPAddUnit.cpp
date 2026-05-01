#include "components/functional_units/FPAddUnit.h"

FPAddUnit::FPAddUnit(RegisterFileEntry *regFile)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), registerFile(regFile)
{
}

double FPAddUnit::calculateResult(const DecodedInstruction &inst)
{
    double v1 = registerFile[inst.src1].value;
    double v2 = registerFile[inst.src2].value;

    switch (inst.op)
    {
    case 6: // fadd
        return v1 + v2;
    case 7: // fsub
        return v1 - v2;
    default:
        return 0;
    }
}
