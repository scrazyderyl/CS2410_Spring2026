#ifndef TYPES_INSTRUCTION_H
#define TYPES_INSTRUCTION_H

#include <string>
#include <cstdint>
#include <map>

#include <ArchitecturalRegister.h>

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
    enum
    {
        NOP = 0,
        FLD = 1,
        FSD = 2,
        ADD = 3,
        ADDI = 4,
        SLT = 5,
        FADD = 6,
        FSUB = 7,
        FMUL = 8,
        FDIV = 9,
        BNE = 10
    } opcode;

    static constexpr double MISPREDICTION_SENTINEL = 2410.0;

    int address = 0;
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
        case NOP: // nop
            return "nop";
        case FLD: // fld
            return instStr + dest.toString() + ", " + std::to_string((int)imm) + "(" + src1.toString() + ")";
        case FSD: // fsd
            return instStr + src2.toString() + ", " + std::to_string((int)imm) + "(" + src1.toString() + ")";
        case ADD: // add
        case SLT: // slt
        case FADD: // fadd
        case FSUB: // fsub
        case FMUL: // fmul
        case FDIV: // fdiv
            return instStr + dest.toString() + ", " + src1.toString() + ", " + src2.toString();
        case ADDI: // addi
            return instStr + dest.toString() + ", " + src1.toString() + ", " + std::to_string((int)imm);
        case BNE: // bne
            return instStr + src1.toString() + ", " + src2.toString() + ", " + std::to_string((int)imm);
        default:
            return "Invalid op code";
        }
    }
};

#endif // TYPES_INSTRUCTION_H
