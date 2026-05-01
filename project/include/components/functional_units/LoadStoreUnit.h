#ifndef COMPONENTS_LOADSTOREUNIT_H
#define COMPONENTS_LOADSTOREUNIT_H

#include "components/FunctionalUnit.h"
#include "types/DecodedInstruction.h"
#include "types/RegisterFileEntry.h"

#include <vector>

class LoadStoreUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_LOAD_RS = 2;
    static constexpr int NUM_STORE_RS = 2;
    static constexpr int NUM_RS = NUM_LOAD_RS + NUM_STORE_RS;
    static constexpr int LATENCY = 1;
    static constexpr bool PIPELINED = true;

    LoadStoreUnit(double *dataMemoryPtr, RegisterFileEntry *regFile);

    virtual bool reserve(const DecodedInstruction &inst, size_t ROBIndex) override;

private:
    double *dataMemory;
    RegisterFileEntry *registerFile;

    virtual double calculateResult(const DecodedInstruction &inst) override;
};

#endif // COMPONENTS_LOADSTOREUNIT_H
