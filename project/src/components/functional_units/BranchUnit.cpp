#include "components/functional_units/BranchUnit.h"

BranchUnit::BranchUnit(const std::vector<ReservationStation *> &rs, BranchPredictor *bp, RegisterFileEntry *regFile)
    : FunctionalUnit(rs, PIPELINED, LATENCY), predictor(bp), registerFile(regFile)
{
}

double BranchUnit::calculateResult(const DecodedInstruction &inst)
{
    double v1 = registerFile[inst.src1].value;
    double v2 = registerFile[inst.src2].value;
    bool taken = (v1 != v2);

    // Update branch predictor BHT and BTB once that's implemented

    return 0;
}
