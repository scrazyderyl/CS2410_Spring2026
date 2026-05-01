#ifndef COMPONENTS_BRANCHUNIT_H
#define COMPONENTS_BRANCHUNIT_H

#include "components/FunctionalUnit.h"
#include "components/BranchPredictor.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class BranchUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_RS = 2;
    static constexpr int LATENCY = 1;
    static constexpr bool PIPELINED = true;

    BranchUnit(BranchPredictor *bp, RegisterFileEntry *regFile);

private:
    BranchPredictor *predictor;

    RegisterFileEntry *registerFile;

    virtual double calculateResult(const DecodedInstruction &inst) override;
};

#endif // COMPONENTS_BRANCHUNIT_H
