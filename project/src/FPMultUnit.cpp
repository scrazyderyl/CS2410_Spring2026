#include "components/functional_units/FPMultUnit.h"

FPMultUnit::FPMultUnit(RegisterFileEntry *regFile)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), registerFile(regFile)
{
}

double FPMultUnit::calculateResult(const DecodedInstruction &inst)
{
    double v1 = registerFile[inst.src1].value;
    double v2 = registerFile[inst.src2].value;
    
    return v1 * v2;
}
