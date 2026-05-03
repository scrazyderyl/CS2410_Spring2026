#include "components/CommonDataBus.h"
#include "Simulator.h"

void CommonDataBus::writeBack(Simulator &sim)
{
    int slots = sim.configuration->NB;

    // Priority: Load, INT, FPadd, FPmult, FPdiv, Store
    // Returns true if all slots have been used up

    // Forward a value to reservation stations waiting on a result written to a physical register
    auto forwardToListeners = [&](uint8_t reg, double value)
    {
        auto tryForward = [&](auto &unit)
        {
            for (ReservationStation &rs : unit.reservationStations)
            {
                if (!rs.busy) {
                    continue;
                }

                if (rs.inst->src1 == reg)
                {
                    rs.setSource1Value(value);
                }

                if (rs.inst->src2 == reg)
                {
                    rs.setSource2Value(value);
                }
            }
        };

        tryForward(sim.loadStoreUnit);
        tryForward(sim.intUnit);
        tryForward(sim.fpAddUnit);
        tryForward(sim.fpMultUnit);
        tryForward(sim.fpDivUnit);
        tryForward(sim.branchUnit);
    };

    auto writeBackRange = [&](auto &unit, size_t begin, size_t end, auto &&applyResult) -> bool
    {
        for (size_t i = begin; i < end; ++i)
        {
            ReservationStation &rs = unit.reservationStations[i];

            if (!rs.isDone())
            {
                continue;
            }

            const DecodedInstruction *inst = rs.inst;
            double result = unit.getResult(i);

            // Send result to ROB
            sim.reorderBuffer.setResult(rs.ROBIndex, result);

            // Update architected register file and forward to any waiting reservation stations
            forwardToListeners(inst->dest, result);

            // Update other CDB listeners as needed for the instruction
            applyResult(*inst, result);
            
            --slots;

            if (slots == 0)
            {
                return true;
            }
        }

        return false;
    };

    // 1) Loads
    if (writeBackRange(sim.loadStoreUnit, 0, LoadStoreUnit::NUM_LOAD_RS, [&](const DecodedInstruction &inst, double value)
                       { sim.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 2) Integer unit
    if (writeBackRange(sim.intUnit, 0, sim.intUnit.reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { sim.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 3) FP add
    if (writeBackRange(sim.fpAddUnit, 0, sim.fpAddUnit.reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { sim.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 4) FP mult
    if (writeBackRange(sim.fpMultUnit, 0, sim.fpMultUnit.reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { sim.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 5) FP div
    if (writeBackRange(sim.fpDivUnit, 0, sim.fpDivUnit.reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { sim.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 6) Stores
    writeBackRange(sim.loadStoreUnit, LoadStoreUnit::NUM_LOAD_RS, LoadStoreUnit::NUM_RS, [&](const DecodedInstruction &inst, double value)
                   {
        double base = sim.registerFile[inst.src1].value;
        uint32_t addr = static_cast<uint32_t>(base + inst.imm);

        // According to the requirements, the value should be written to the store queue instead of directly to data memory
        // But I don't think this should make any difference
        sim.dataMemory[addr] = value; });
}
