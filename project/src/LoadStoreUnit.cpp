#include "components/functional_units/LoadStoreUnit.h"

LoadStoreUnit::LoadStoreUnit(double *dataMemoryPtr, RegisterFileEntry *regFile)
    : FunctionalUnit(NUM_RS, LATENCY, PIPELINED), dataMemory(dataMemoryPtr), registerFile(regFile)
{
}

bool LoadStoreUnit::reserve(const DecodedInstruction &inst, size_t ROBIndex)
{
    switch (inst.op)
    {
    case 1: // fld
        for (size_t i = 0; i < NUM_LOAD_RS; i++)
        {
            auto &rs = reservationStations[i];

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
            auto &rs = reservationStations[i];

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

double LoadStoreUnit::calculateResult(const DecodedInstruction &inst)
{
    double base = registerFile[inst.src1].value;
    int addr = (uint32_t)(base + inst.imm);

    switch (inst.op)
    {
    case 1: // fld
        return dataMemory[addr];
    case 2: // fsd
        dataMemory[addr] = registerFile[inst.src2].value;
        return 0;
    default:
        return 0;
    }
}
