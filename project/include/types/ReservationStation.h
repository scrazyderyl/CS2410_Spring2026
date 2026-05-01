#ifndef TYPES_RESERVATIONSTATION_H
#define TYPES_RESERVATIONSTATION_H

#include <cstdint>

#include "types/DecodedInstruction.h"

struct ReservationStation
{
    bool busy = false;
    int cycles_left = -1;
    const DecodedInstruction *inst = nullptr;
    int ROBIndex = -1;

    inline bool isReadyToExecute()
    {
        return busy && cycles_left == -1;
    }

    inline bool isExecuting()
    {
        return busy && cycles_left > 0;
    }

    inline bool isDone()
    {
        return busy && cycles_left == 0;
    }

    inline void reserveInstruction(const DecodedInstruction &decodedInst, int robIndex)
    {
        busy = true;
        cycles_left = -1;
        inst = &decodedInst;
        ROBIndex = robIndex;
    }
};

#endif // TYPES_RESERVATIONSTATION_H
