#include "components/functional_units/LoadStoreUnit.h"

LoadStoreUnit::LoadStoreUnit(const std::vector<ReservationStation *> &rs, double *dataMemoryPtr, RegisterFileEntry *regFile)
    : FunctionalUnit(rs, PIPELINED, LATENCY), dataMemory(dataMemoryPtr), registerFile(regFile)
{
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
    }
}
