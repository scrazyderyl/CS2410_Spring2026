#include "components/ReorderBuffer.h"
#include "Simulator.h"

ReorderBuffer::ReorderBuffer(Simulator &sim) : sim(sim)
{
    buffer.resize(sim.configuration->NR);
}

int ReorderBuffer::add(ArchitecturalRegister dest, int physRegIndex)
{
    // Check if ROB is full
    if (head == tail)
    {
        return -1;
    }

    int index = tail;

    // Add entry at tail
    ROBEntry newEntry = {
        .dest = dest,
        .physRegIndex = physRegIndex,
        .result = 0.0,
        .done = false
    };
    
    buffer[index] = newEntry;

    // Update tail
    tail = (tail + 1) % sim.configuration->NR;

    return index;
}

bool ReorderBuffer::isRegisterReady(int physRegIndex)
{
    // Zero register is always ready
    if (physRegIndex == NUM_PHYS_REG)
    {
        return true;
    }

    // Search ROB for physical register and check if it's done
    for (int i = head; i != tail; i = (i + 1) % sim.configuration->NR)
    {
        if (buffer[i].physRegIndex == physRegIndex)
        {
            return buffer[i].done;
        }
    }

    // This means it's a source register whose value is directly loaded in from the architectural register file
    return true;
}

void ReorderBuffer::setResult(int index, double result)
{
    buffer[index].result = result;
    buffer[index].done = true;
}

bool ReorderBuffer::isEmpty()
{
    return head == tail;
}

void ReorderBuffer::commit()
{
    int commitCount = 0;

    while (head != tail && commitCount < sim.configuration->NC && buffer[head].done)
    {
        // Write result to architectural register file
        ROBEntry &entry = buffer[head];

        // Instructions that don't write to a destination register will have dest set to X0 which is read-only
        if (entry.dest.type != ArchitecturalRegister::X || entry.dest.num != 0)
        {
            sim.architecturalRegisterFile.setValue(entry.dest, entry.result);
            sim.freePhysicalRegisters.push(entry.physRegIndex);
        }

        // Update head
        head = (head + 1) % sim.configuration->NR;

        // Release physical register

        commitCount++;
    }
}
