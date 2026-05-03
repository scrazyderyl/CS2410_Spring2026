#include "components/ReorderBuffer.h"
#include "Simulator.h"

ReorderBuffer::ReorderBuffer(Simulator &sim) : sim(sim)
{
    buffer.resize(sim.configuration->NR);
}

bool ReorderBuffer::add(ArchitecturalRegister dest, int physRegIndex)
{
    // Check if ROB is full
    if (head == tail)
    {
        return false;
    }

    // Add entry at tail
    ROBEntry newEntry = {
        .dest = dest,
        .physRegIndex = physRegIndex,
        .result = 0.0,
        .done = false
    };
    
    buffer[tail] = newEntry;

    // Update tail
    tail = (tail + 1) % sim.configuration->NR;

    return true;
}

void ReorderBuffer::setResult(int index, double result)
{
    buffer[index].result = result;
    buffer[index].done = true;
}

void ReorderBuffer::commit()
{
    int commitCount = 0;

    while (head != tail && commitCount < sim.configuration->NC && buffer[head].done)
    {
        // Write result to architectural register file
        ROBEntry &entry = buffer[head];
        sim.architecturalRegisterFile.setValue(entry.dest, entry.result);

        // Update head
        head = (head + 1) % sim.configuration->NR;

        // Release physical register

        commitCount++;
    }
}
