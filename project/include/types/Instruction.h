#ifndef COMPONENTS_TYPES_INSTRUCTION_H
#define COMPONENTS_TYPES_INSTRUCTION_H

#include <string>

#include <types/ArchitecturalRegister.h>

inline const std::map<std::string, uint8_t> instructionOpcodes = {
    {"nop", 0},
    {"fld", 1},
    {"fsd", 2},
    {"add", 3},
    {"addi", 4},
    {"slt", 5},
    {"fadd", 6},
    {"fsub", 7},
    {"fmul", 8},
    {"fdiv", 9},
    {"bne", 10}};

struct Instruction
{
    std::string text; // Original instruction text
    uint8_t opcode;
    ArchitecturalRegister rd;
    ArchitecturalRegister rs;
    ArchitecturalRegister rt;
    int32_t immediate;
};

#endif // COMPONENTS_TYPES_INSTRUCTION_H
