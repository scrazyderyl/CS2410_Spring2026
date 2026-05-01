#ifndef TYPES_ROBENTRY_H
#define TYPES_ROBENTRY_H

#include <types/ArchitecturalRegister.h>

#include <cstdint>

struct ROBEntry
{
    // Could also include result value and instruction info for debugging but not necessary for execution
    bool busy; // Technically this is also unnecessary since no flushing happens
    ArchitecturalRegister dest;
};

#endif // TYPES_ROBENTRY_H
