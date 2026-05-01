#ifndef COMPONENTS_FPDIVUNIT_H
#define COMPONENTS_FPDIVUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class FPDivUnit : public FunctionalUnit
{
public:
    static constexpr int LATENCY = 6;
    static constexpr bool PIPELINED = false;

    FPDivUnit(const std::vector<ReservationStation*> &rs, RegisterFileEntry *regFile);

private:
    virtual double calculateResult(DecodedInstruction &inst) override;
    RegisterFileEntry *registerFile;
};

#endif // COMPONENTS_FPDIVUNIT_H
