#ifndef TYPES_BTBENTRY_H
#define TYPES_BTBENTRY_H

#include <cstdint>

struct BTBEntry
{
    int targetAddress;
    bool valid = false;
};

#endif // TYPES_BTBENTRY_H
