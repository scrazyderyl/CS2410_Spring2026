#ifndef COMPONENTS_FUNCTIONALUNIT_H
#define COMPONENTS_FUNCTIONALUNIT_H

#include "types/DecodedInstruction.h"
#include "types/ReservationStation.h"

#include <vector>

class FunctionalUnit
{
public:
    std::vector<ReservationStation> reservationStations;
    bool pipelined;
    int latency;

    FunctionalUnit(size_t numReservationStations, int latency, bool pipelined = true) : reservationStations(numReservationStations), pipelined(pipelined), latency(latency) {}
    virtual ~FunctionalUnit() = default;

    // Try to reserve a decoded instruction
    // By default, this method can reserve any reservation station that is not busy
    // Can be overriden for functional units with different reservation stations for different instructions
    virtual bool reserve(const DecodedInstruction &inst, size_t ROBIndex) {
        for (auto &rs : reservationStations)
        {
            if (!rs.busy)
            {
                rs.reserveInstruction(inst, ROBIndex);
                return true;
            }
        }

        return false;
    }

    // Simulate one cycle
    void execute();

    // Get the result of executing the decoded instruction
    // Once this is called the reservation station is freed up
    // Does not do any safety checks, it assumes those have been done by the caller
    double getResult(size_t rsIndex);
private:
    // Do the actual result calculation for the instruction
    // This method also does other operations associated with the instruction
    virtual double calculateResult(const DecodedInstruction &inst) = 0;
};

#endif // COMPONENTS_FUNCTIONALUNIT_H
