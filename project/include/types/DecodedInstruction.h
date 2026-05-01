#ifndef TYPES_DECODEDINSTRUCTION_H
#define TYPES_DECODEDINSTRUCTION_H

#include <cstdint>

struct DecodedInstruction
{
    uint8_t op;
    uint8_t dest;
    uint8_t src1;
    uint8_t src2;
    double imm;
};

#endif // TYPES_DECODEDINSTRUCTION_H