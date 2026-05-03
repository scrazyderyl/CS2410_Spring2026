#ifndef TYPES_RESERVATIONSTATION_H
#define TYPES_RESERVATIONSTATION_H

#include <cstddef>
#include <cstdint>

#include "types/DecodedInstruction.h"

struct ReservationStation
{
    bool busy = false;
    int cycles_left = -1;
    const DecodedInstruction *inst = nullptr;
    int ROBIndex = -1;

    // Store received source values so that physical registers can be recycled on instruction commit
    bool src1_ready = false;
    bool src2_ready = false;
    double src1_value = 0.0;
    double src2_value = 0.0;

    inline bool isReadyToExecute()
    {
        return busy && cycles_left == -1 && src1_ready && src2_ready;
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

    inline void setSource1Value(double value)
    {
        // Since physical registers are freed on commit, other instructions may forward to this source reigster
        // Need to make sure value is only written once
        if (!src1_ready)
        {
            src1_ready = true;
            src1_value = value;
        }
    }

    inline void setSource2Value(double value)
    {
        if (!src2_ready)
        {
            src2_ready = true;
            src2_value = value;
        }
    }
};

#endif // TYPES_RESERVATIONSTATION_H
