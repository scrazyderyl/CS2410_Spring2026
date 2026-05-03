#ifndef COMPONENTS_REORDERBUFFER_H
#define COMPONENTS_REORDERBUFFER_H

#include <types/ROBEntry.h>
#include <vector>
#include <cstdint>

class Simulator;

class ReorderBuffer
{
public:
    ReorderBuffer(Simulator &sim);

    bool add(ArchitecturalRegister dest, int physRegIndex);

    void setResult(int index, double result);

    void commit();

private:
    Simulator &sim;
    std::vector<ROBEntry> buffer;

    int head = 0;
    int tail = 0; // Actually points to the next slot to populate
};

#endif // COMPONENTS_REORDERBUFFER_H
