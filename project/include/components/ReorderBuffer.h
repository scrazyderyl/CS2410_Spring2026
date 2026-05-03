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

    // Returns index of new entry, or -1 if ROB is full
    int add(ArchitecturalRegister dest, int physRegIndex = -1);

    void setResult(int index, double result);

    void commit();

private:
    Simulator &sim;
    std::vector<ROBEntry> buffer;

    int head = 0;
    int tail = 0; // Actually points to the next slot to populate
};

#endif // COMPONENTS_REORDERBUFFER_H
