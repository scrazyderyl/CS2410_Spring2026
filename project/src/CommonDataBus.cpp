#include "components/CommonDataBus.h"
#include "Simulator.h"

void CommonDataBus::writeBack(Simulator &sim)
{
    int slots = sim.configuration->NB;

    // Handle branches
    for (size_t i = 0; i < sim.branchUnit.reservationStations.size(); i++)
    {
        ReservationStation &rs = sim.branchUnit.reservationStations[i];

        if (rs.isDone())
        {
            sim.branchUnit.getResult(i);

            // As mentioned in the notes for the branch predictor,
            // the branch predictor doesn't need to be updated with the branch outcome
            // This just notifies the fetch unit that the misprediction has been resolved
        }
    }

    // Priority: Load, INT, FPadd, FPmult, FPdiv, Store
    // Returns true if all slots have been used up
    auto writeBackRange = [&](auto &unit, size_t begin, size_t end, auto &&applyResult) -> bool
    {
        for (size_t i = begin; i < end; ++i)
        {
            if (!unit.reservationStations[i].isDone())
            {
                continue;
            }

            const DecodedInstruction *inst = unit.reservationStations[i].inst;
            double result = unit.getResult(i);
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
