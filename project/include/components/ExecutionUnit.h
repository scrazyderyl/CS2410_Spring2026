#ifndef COMPONENTS_EXECUTIONUNIT_H
#define COMPONENTS_EXECUTIONUNIT_H

#include "types/DecodedInstruction.h"
#include "types/ReservationStation.h"

#include <vector>

class ExecutionUnit
{
public:
    std::vector<ReservationStation*> reservationStations;

    // Try to reserve a decoded instruction
    virtual bool reserve(const DecodedInstruction &inst) = 0;

    // Simulate one cycle
    virtual void execute() = 0;

    // Get the result of executing the decoded instruction
    // Once this is called the reservation station is freed up 
    virtual double getResult(size_t rsIndex) = 0;
};

#endif // COMPONENTS_EXECUTIONUNIT_H
