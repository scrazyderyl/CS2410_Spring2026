#include "components/functional_units/IntegerUnit.h"
#include "types/RegisterFileEntry.h"

IntegerUnit::IntegerUnit(const std::vector<ReservationStation *> &rs, RegisterFileEntry *regFile)
    : FunctionalUnit(rs, PIPELINED, LATENCY), registerFile(regFile)
{
}

double IntegerUnit::calculateResult(DecodedInstruction &inst)
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
        return static_cast<double>(static_cast<int>(v1) < static_cast<int>(v2));
    }
}
