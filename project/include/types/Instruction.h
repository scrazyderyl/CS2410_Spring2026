#ifndef TYPES_INSTRUCTION_H
#define TYPES_INSTRUCTION_H

#include <string>
#include <cstdint>
#include <map>

#include <types/ArchitecturalRegister.h>

inline const std::map<std::string, uint8_t> instToOpCode = {
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

inline const std::map<uint8_t, std::string> opCodeToInst = {
    {0, "nop"},
    {1, "fld"},
    {2, "fsd"},
    {3, "add"},
    {4, "addi"},
    {5, "slt"},
    {6, "fadd"},
    {7, "fsub"},
    {8, "fmul"},
    {9, "fdiv"},
    {10, "bne"}};

struct Instruction
{
    uint8_t op;
    ArchitecturalRegister dest;
    ArchitecturalRegister src1;
    ArchitecturalRegister src2;
    double imm;

    // Represent as string
    std::string toString() const
    {
        std::string instStr = opCodeToInst.at(op) + " ";
        
        switch (op)
        {
        case 0: // nop
            return "nop";
        case 1: // fld
        case 2: // fsd
            return instStr + dest.toString() + ", " + std::to_string((int)imm) + "(" + src1.toString() + ")";
        case 3: // add
        case 5: // slt
        case 6: // fadd
        case 7: // fsub
        case 8: // fmul
        case 9: // fdiv
            return instStr + dest.toString() + ", " + src1.toString() + ", " + src2.toString();
        case 4: // addi
            return instStr + dest.toString() + ", " + src1.toString() + ", " + std::to_string((int)imm);
        case 10: // bne
            return instStr + src1.toString() + ", " + src2.toString() + ", " + std::to_string((int)imm);
        default:
            return "Invalid op code";
        }
    }
};

#endif // TYPES_INSTRUCTION_H
