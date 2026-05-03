#include "components/FunctionalUnit.h"

#include <stddef.h>

void FunctionalUnit::execute()
{
    if (pipelined)
    {
        // Start at most one ready reservation station each cycle
        for (ReservationStation &rs : reservationStations)
        {
            if (rs.isReadyToExecute())
            {
                rs.cycles_left = latency;
                break;
            }
        }

        // Advance every instruction that is already executing
        for (ReservationStation &rs : reservationStations)
        {
            if (rs.isExecuting())
            {
                rs.cycles_left--;
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

    return calculateResult(rs);
}