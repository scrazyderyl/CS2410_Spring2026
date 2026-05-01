#ifndef COMPONENTS_FUNCTIONALUNIT_H
#define COMPONENTS_FUNCTIONALUNIT_H

#include "types/DecodedInstruction.h"
#include "types/ReservationStation.h"

#include <vector>

class FunctionalUnit
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

#endif // COMPONENTS_FUNCTIONALUNIT_H
