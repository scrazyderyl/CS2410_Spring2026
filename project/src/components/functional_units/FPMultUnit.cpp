#include "components/functional_units/FPMultUnit.h"

FPMultUnit::FPMultUnit(const std::vector<ReservationStation *> &rs, RegisterFileEntry *regFile)
    : FunctionalUnit(rs, PIPELINED, LATENCY), registerFile(regFile)
{
}

double FPMultUnit::calculateResult(DecodedInstruction &inst)
{
    double v1 = registerFile[inst.src1].value;
    double v2 = registerFile[inst.src2].value;
    
    return v1 * v2;
}
