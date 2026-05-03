#ifndef TYPES_ROBENTRY_H
#define TYPES_ROBENTRY_H

#include <types/ArchitecturalRegister.h>

#include <cstdint>

struct ROBEntry
{
    ArchitecturalRegister dest;
    // Which physical register dest is mapped to, if any
    int physRegIndex = -1;
    double result = 0.0;
    bool done = false;
};

#endif // TYPES_ROBENTRY_H
