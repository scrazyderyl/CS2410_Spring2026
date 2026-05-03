#ifndef COMPONENTS_INSTRUCTIONFETCHUNIT_H
#define COMPONENTS_INSTRUCTIONFETCHUNIT_H

#include <vector>

#include "types/Instruction.h"

class Simulator;

class InstructionFetchUnit
{
public:
    InstructionFetchUnit(Simulator &sim);

    std::vector<Instruction> fetch();

    const std::vector<Instruction> &getFetchQueue();

    void consumeFetchQueue(std::size_t count);

    bool isFetchEnabled();

    void setFetchEnabled(bool enabled);

    bool isQueueEmpty();

private:
    Simulator &sim;
    std::vector<Instruction> fetchQueue;

    unsigned int nextFetchPC = 0;
    bool fetchEnabled = true;
};

#endif // COMPONENTS_INSTRUCTIONFETCHUNIT_H
