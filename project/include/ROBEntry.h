#ifndef TYPES_ROBENTRY_H
#define TYPES_ROBENTRY_H

#include <ArchitecturalRegister.h>

#include <cstdint>

struct ROBEntry
{
    ArchitecturalRegister dest{ArchitecturalRegister::X, 0};
    int physRegIndex = -1;
    int oldPhysRegIndex = -1;
    double result = 0.0;
    bool done = false;
};

#endif // TYPES_ROBENTRY_H
