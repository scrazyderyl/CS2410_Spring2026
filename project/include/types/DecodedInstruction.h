#ifndef TYPES_DECODEDINSTRUCTION_H
#define TYPES_DECODEDINSTRUCTION_H

#include <cstdint>

#include <types/ArchitecturalRegister.h>

struct DecodedInstruction
{
    uint8_t op;
    uint8_t dest;
    uint8_t src1;
    uint8_t src2;
    double imm;
    
    // Reorder buffer needs this information since it's not passed the original instruction
    ArchitecturalRegister archDest = ArchitecturalRegister{ArchitecturalRegister::X, 0};
};

#endif // TYPES_DECODEDINSTRUCTION_H