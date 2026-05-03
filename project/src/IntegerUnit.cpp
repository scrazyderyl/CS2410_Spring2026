#include "components/functional_units/IntegerUnit.h"
#include "types/RegisterFileEntry.h"

IntegerUnit::IntegerUnit(RegisterFileEntry *regFile)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), registerFile(regFile)
{
}

double IntegerUnit::calculateResult(const DecodedInstruction &inst)
{
    double v1 = registerFile[inst.src1].value;
    double v2 = registerFile[inst.src2].value;

    switch (inst.op)
    {
    case 3: // add
        return v1 + v2;
    case 4: // addi
        return v1 + inst.imm;
    case 5: // slt
        return v1 < v2 ? 1 : 0;
    default:
        return 0;
    }
}
