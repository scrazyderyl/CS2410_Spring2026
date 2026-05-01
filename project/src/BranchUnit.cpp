#include "components/functional_units/BranchUnit.h"

BranchUnit::BranchUnit(RegisterFileEntry *regFile)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), registerFile(regFile)
{
}

double BranchUnit::calculateResult(const DecodedInstruction &inst)
{
    double v1 = registerFile[inst.src1].value;
    double v2 = registerFile[inst.src2].value;
    bool taken = (v1 != v2);

    // Return 1 if branch is taken, 0 if not taken
    return taken ? 1.0 : 0.0;
}
