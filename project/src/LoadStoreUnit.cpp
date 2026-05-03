#include "components/functional_units/LoadStoreUnit.h"

LoadStoreUnit::LoadStoreUnit(double *dataMemoryPtr)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), dataMemory(dataMemoryPtr)
{
}

bool LoadStoreUnit::reserve(const DecodedInstruction &inst, size_t ROBIndex)
{
    switch (inst.op)
    {
    case 1: // fld
        for (size_t i = 0; i < NUM_LOAD_RS; i++)
        {
            ReservationStation &rs = reservationStations[i];

            if (!rs.busy)
            {
                rs.reserveInstruction(inst, ROBIndex);
                return true;
            }
        }

        break;
    case 2: // fsd
        for (size_t i = NUM_LOAD_RS; i < NUM_RS; i++)
        {
            ReservationStation &rs = reservationStations[i];

            if (!rs.busy)
            {
                rs.reserveInstruction(inst, ROBIndex);
                return true;
            }
        }

        break;
    default:
        return false;
    }

    return false;
}

double LoadStoreUnit::calculateResult(const ReservationStation &rs)
{
    double base = rs.src1_value;
    int addr = (uint32_t)(base + rs.inst->imm);

    switch (rs.inst->op)
    {
    case 1: // fld
        return dataMemory[addr];
    case 2: // fsd
        return rs.src2_value;
    default:
        return 0;
    }
}
