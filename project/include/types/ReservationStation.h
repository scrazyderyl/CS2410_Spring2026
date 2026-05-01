#ifndef TYPES_RESERVATIONSTATION_H
#define TYPES_RESERVATIONSTATION_H

#include <cstdint>

#include "types/DecodedInstruction.h"

struct ReservationStation
{
    bool busy;
    int cycles_left;
    DecodedInstruction inst;
    int ROBIndex;
};

#endif // TYPES_RESERVATIONSTATION_H
