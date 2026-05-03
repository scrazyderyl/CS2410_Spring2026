#include "components/InstructionDispatcher.h"
#include "Simulator.h"

#include "components/functional_units/LoadStoreUnit.h"

#include <algorithm>

// Check whether a physical register still has an in-flight producer in any FU.
bool InstructionDispatcher::isSourceRegisterReady(uint8_t physReg) const
{
    if (physReg == NUM_PHYS_REG)
    {
        return true;
    }

    auto isPendingInUnit = [physReg](const FunctionalUnit &unit)
    {
        for (const ReservationStation &rs : unit.reservationStations)
        {
            if (rs.busy && rs.inst->dest == physReg)
            {
                return true;
            }
        }

        return false;
    };

    return isPendingInUnit(simulator.loadStoreUnit) || isPendingInUnit(simulator.intUnit) ||
           isPendingInUnit(simulator.fpAddUnit) || isPendingInUnit(simulator.fpMultUnit) ||
           isPendingInUnit(simulator.fpDivUnit) || isPendingInUnit(simulator.branchUnit);
}

InstructionDispatcher::InstructionDispatcher(Simulator &sim) : simulator(sim) {}

void InstructionDispatcher::registerInstructionExecuter(uint8_t op, FunctionalUnit *executor)
{
    executors[op] = executor;
}

void InstructionDispatcher::dispatch()
{
    std::size_t dispatchLimit = std::min<std::size_t>(simulator.configuration->NW, simulator.instructionQueue.size());
    std::size_t dispatchedCount = 0;

    for (std::size_t i = 0; i < dispatchLimit; ++i)
    {
        // Look up functional unit that handles this instruction
        DecodedInstruction &inst = simulator.instructionQueue[i];
        FunctionalUnit *executor = executors[inst.op];

        // Check if a reservation station is available
        ReservationStation *availableRS = executor->getAvailableRS(inst);

        if (availableRS == nullptr)
        {
            simulator.rs_stalls++;
            break;
        }

        // Check if there is space in the ROB
        int robIndex = simulator.reorderBuffer.add(inst.archDest, inst.dest);

        if (robIndex == -1)
        {
            simulator.rob_stalls++;
            break;
        }

        // Reserve the instruction in the reservation station
        availableRS->reserveInstruction(inst, robIndex);
        
        if (isSourceRegisterReady(inst.src1))
        {
            availableRS->setSource1Value(simulator.registerFile[inst.src1].value);
        } else {
            availableRS->src1_ready = false;
        }

        if (isSourceRegisterReady(inst.src2))
        {
            availableRS->setSource2Value(simulator.registerFile[inst.src2].value);
        } else {
            availableRS->src2_ready = false;
        }

        ++dispatchedCount;
    }

    // Remove dispatched instructions from the instruction queue
    if (dispatchedCount > 0)
    {
        simulator.instructionQueue.erase(simulator.instructionQueue.begin(), simulator.instructionQueue.begin() + dispatchedCount);
    }
}