#ifndef COMPONENTS_INSTRUCTIONFETCHUNIT_H
#define COMPONENTS_INSTRUCTIONFETCHUNIT_H

#include <vector>

#include "types/Instruction.h"

class Simulator;

class InstructionFetchUnit
{
public:
    InstructionFetchUnit(Simulator &sim);

    std::vector<Instruction> fetch(uint pc);

    void setFetchEnabled(bool enabled);

private:
    Simulator &sim;
    std::vector<Instruction> fetchQueue;
    bool fetchEnabled = true;
};

#endif // COMPONENTS_INSTRUCTIONFETCHUNIT_H
