#ifndef TYPES_ARCHITECTURALREGISTERFILE_H
#define TYPES_ARCHITECTURALREGISTERFILE_H

#include <types/ArchitecturalRegister.h>

class ArchitecturalRegisterFile
{
public:
    static const int NUM_X_REGS = 32;
    static const int NUM_F_REGS = 32;

    double getValue(const ArchitecturalRegister &reg) const
    {
        return reg.type == ArchitecturalRegister::X ? xRegisters[reg.num] : fRegisters[reg.num];
    }

    void setValue(const ArchitecturalRegister &reg, double value)
    {
        if (reg.type == ArchitecturalRegister::X)
        {
            xRegisters[reg.num] = value;
        }
        else
        {
            fRegisters[reg.num] = value;
        }
    }

private:
    double xRegisters[NUM_X_REGS] = {0};
    double fRegisters[NUM_F_REGS] = {0};
};

#endif // TYPES_ARCHITECTURALREGISTERFILE_H