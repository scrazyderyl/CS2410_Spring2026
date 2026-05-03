#include "components/CommonDataBus.h"
#include "Simulator.h"

void CommonDataBus::writeBack(Simulator &simulator)
{
    int slots = simulator.configuration->NB;

    // Handle branches
    for (size_t i = 0; i < simulator.branchUnit->reservationStations.size(); i++)
    {
        ReservationStation &rs = simulator.branchUnit->reservationStations[i];
        if (rs.isDone())
        {
            double result = simulator.branchUnit->getResult(i);
            // Todo: update branch predictor with result and possibly flush instructions if mispredicted
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
    if (writeBackRange(*simulator.loadStoreUnit, 0, LoadStoreUnit::NUM_LOAD_RS, [&](const DecodedInstruction &inst, double value)
                       { simulator.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 2) Integer unit
    if (writeBackRange(*simulator.intUnit, 0, simulator.intUnit->reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { simulator.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 3) FP add
    if (writeBackRange(*simulator.fpAddUnit, 0, simulator.fpAddUnit->reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { simulator.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 4) FP mult
    if (writeBackRange(*simulator.fpMultUnit, 0, simulator.fpMultUnit->reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { simulator.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 5) FP div
    if (writeBackRange(*simulator.fpDivUnit, 0, simulator.fpDivUnit->reservationStations.size(), [&](const DecodedInstruction &inst, double value)
                       { simulator.registerFile[inst.dest].value = value; }))
    {
        return;
    }

    // 6) Stores
    writeBackRange(*simulator.loadStoreUnit, LoadStoreUnit::NUM_LOAD_RS, LoadStoreUnit::NUM_RS, [&](const DecodedInstruction &inst, double value)
                   {
        double base = simulator.registerFile[inst.src1].value;
        uint32_t addr = static_cast<uint32_t>(base + inst.imm);

        if (addr < MAX_MEM_SIZE)
        {
            simulator.dataMemory[addr] = value;
        } });
}
