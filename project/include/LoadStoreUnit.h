#ifndef COMPONENTS_LOADSTOREUNIT_H
#define COMPONENTS_LOADSTOREUNIT_H

#include "FunctionalUnit.h"
#include "DecodedInstruction.h"

#include <vector>

class LoadStoreUnit : public FunctionalUnit
{
public:
    static constexpr int NUM_LOAD_RS = 2;
    static constexpr int NUM_STORE_RS = 2;
    static constexpr int NUM_RS = NUM_LOAD_RS + NUM_STORE_RS;
    static constexpr int LATENCY = 1;
    static constexpr bool PIPELINED = true;

    LoadStoreUnit(double *dataMemoryPtr);

    virtual ReservationStation* getAvailableRS(const DecodedInstruction &inst) override;

private:
    double *dataMemory;

    virtual double calculateResult(const ReservationStation &rs) override;
};

#endif // COMPONENTS_LOADSTOREUNIT_H
