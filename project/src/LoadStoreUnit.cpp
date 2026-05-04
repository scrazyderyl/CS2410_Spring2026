#include "LoadStoreUnit.h"
#include "Instruction.h"

LoadStoreUnit::LoadStoreUnit(double *dataMemoryPtr)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), dataMemory(dataMemoryPtr)
{
}

ReservationStation* LoadStoreUnit::getAvailableRS(const DecodedInstruction &inst)
{
    switch (inst.op)
    {
    case Instruction::FLD:
        for (size_t i = 0; i < NUM_LOAD_RS; i++)
        {
            ReservationStation &rs = reservationStations[i];

            if (!rs.busy)
            {
                return &rs;
            }
        }

        break;
    case Instruction::FSD:
        for (size_t i = NUM_LOAD_RS; i < NUM_RS; i++)
        {
            ReservationStation &rs = reservationStations[i];

            if (!rs.busy)
            {
                return &rs;
            }
        }

        break;
    default:
        return nullptr;
    }

    return nullptr;
}

double LoadStoreUnit::calculateResult(const ReservationStation &rs)
{
    double base = rs.src1_value;
    int addr = static_cast<int>(base + rs.inst.imm);

    switch (rs.inst.op)
    {
    case Instruction::FLD:
        return dataMemory[addr];
    case Instruction::FSD:
        return rs.src2_value;
    default:
        return 0;
    }
}
