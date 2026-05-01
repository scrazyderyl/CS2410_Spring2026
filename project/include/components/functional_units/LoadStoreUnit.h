#ifndef COMPONENTS_LOADSTOREUNIT_H
#define COMPONENTS_LOADSTOREUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class LoadStoreUnit : public FunctionalUnit
{
public:
    static constexpr int LATENCY = 1;
    static constexpr bool PIPELINED = true;

    LoadStoreUnit(const std::vector<ReservationStation*> &rs, double *dataMemoryPtr, RegisterFileEntry *regFile);

private:
    double *dataMemory;
    RegisterFileEntry *registerFile;

    virtual double calculateResult(const DecodedInstruction &inst) override;
};

#endif // COMPONENTS_LOADSTOREUNIT_H
