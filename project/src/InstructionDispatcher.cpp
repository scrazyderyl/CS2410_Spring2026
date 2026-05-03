#include "components/InstructionDispatcher.h"
#include "Simulator.h"

#include "components/functional_units/LoadStoreUnit.h"

#include <algorithm>

InstructionDispatcher::InstructionDispatcher(Simulator &sim) : sim(sim) {}

void InstructionDispatcher::registerInstructionExecuter(uint8_t op, FunctionalUnit *executor)
{
    executors[op] = executor;
}

void InstructionDispatcher::dispatch()
{
    std::size_t dispatchLimit = std::min<std::size_t>(sim.configuration->NW, sim.instructionQueue.size());
    std::size_t dispatchedCount = 0;

    for (std::size_t i = 0; i < dispatchLimit; ++i)
    {
        // Look up functional unit that handles this instruction
        DecodedInstruction &inst = sim.instructionQueue[i];
        FunctionalUnit *executor = executors[inst.op];

        // Check if a reservation station is available
        ReservationStation *availableRS = executor->getAvailableRS(inst);

        if (availableRS == nullptr)
        {
            sim.rs_stalls++;
            break;
        }

        // Check if there is space in the ROB
        int robIndex = sim.reorderBuffer.add(inst.archDest, inst.dest);

        if (robIndex == -1)
        {
            sim.rob_stalls++;
            break;
        }

        // Reserve the instruction in the reservation station
        availableRS->reserveInstruction(inst, robIndex);
        
        if (sim.reorderBuffer.isRegisterReady(inst.src1))
        {
            availableRS->setSource1Value(sim.registerFile[inst.src1].value);
        } else {
            availableRS->src1_ready = false;
        }

        if (sim.reorderBuffer.isRegisterReady(inst.src2))
        {
            availableRS->setSource2Value(sim.registerFile[inst.src2].value);
        } else {
            availableRS->src2_ready = false;
        }

        ++dispatchedCount;
    }

    // Remove dispatched instructions from the instruction queue
    if (dispatchedCount > 0)
    {
        sim.instructionQueue.erase(sim.instructionQueue.begin(), sim.instructionQueue.begin() + dispatchedCount);
    }
}