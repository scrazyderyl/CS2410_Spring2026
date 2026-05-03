#include "components/FunctionalUnit.h"

#include <stddef.h>

void FunctionalUnit::execute()
{
    if (pipelined)
    {
        // See if there's an reservation station that had already started executing
        for (ReservationStation &rs : reservationStations)
        {
            if (rs.isExecuting())
            {
                rs.cycles_left--;
                return;
            }
        }

        // Otherwise find a reservation station that is ready to start executing
        for (ReservationStation &rs : reservationStations)
        {
            if (rs.busy)
            {
                rs.cycles_left = latency;
                break;
            }
        }
    }
    else
    {
        // Start executing up to 1 instruction that is ready to execute
        for (ReservationStation &rs : reservationStations)
        {
            if (rs.isReadyToExecute())
            {
                rs.cycles_left = latency;
                break;
            }
        }

        // Decrement cycles for currently executing instructions (including any that were just started)
        for (ReservationStation &rs : reservationStations)
        {
            if (rs.isExecuting())
            {
                rs.cycles_left--;
            }
        }
    }
}

double FunctionalUnit::getResult(size_t rsIndex)
{
    ReservationStation &rs = reservationStations[rsIndex];

    rs.busy = false;

    return calculateResult(*rs.inst);
}