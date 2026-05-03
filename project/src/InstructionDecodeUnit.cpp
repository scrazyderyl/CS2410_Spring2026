#include "components/InstructionDecodeUnit.h"
#include "Simulator.h"

#include <stdint.h>

InstructionDecodeUnit::InstructionDecodeUnit(Simulator &sim) : sim(sim) {}

int InstructionDecodeUnit::allocatePhysicalRegister()
{
	if (sim.freePhysicalRegisters.empty())
	{
		return -1;
	}

	int physReg = sim.freePhysicalRegisters.front();
	sim.freePhysicalRegisters.pop();

	return physReg;
}

int InstructionDecodeUnit::resolveSourceRegister(const ArchitecturalRegister &reg)
{
	// X0 maps to a dummy physical register that's always 0
	if (reg.type == ArchitecturalRegister::X && reg.num == 0)
	{
		return NUM_PHYS_REG;
	}

	// Return the existing mapping if it exists
	auto existing = sim.registerMapTable.find(reg);

	if (existing != sim.registerMapTable.end())
	{
		return existing->second;
	}

	// Otherwise, allocate a new physical register
	int physReg = allocatePhysicalRegister();

	if (physReg < 0)
	{
		// No free physical register available
		return -1;
	}

	sim.registerFile[physReg].value = sim.architecturalRegisterFile.getValue(reg);
	sim.registerMapTable[reg] = physReg;

	return physReg;
}

int InstructionDecodeUnit::renameDestinationRegister(const ArchitecturalRegister &reg)
{
	if (reg.type == ArchitecturalRegister::X && reg.num == 0)
	{
		return NUM_PHYS_REG;
	}

	int physReg = allocatePhysicalRegister();

	if (physReg < 0)
	{
		return -1;
	}

	sim.registerMapTable[reg] = physReg;

	return physReg;
}

void InstructionDecodeUnit::decode()
{
	const std::vector<Instruction> &fetchQueue = sim.instructionFetchUnit.getFetchQueue();

	size_t i = 0;

	for (; i < fetchQueue.size() && static_cast<int>(sim.instructionQueue.size()) < sim.configuration->NI; i++)
	{
		const Instruction &inst = fetchQueue[i];

		// If the last instruction in the fetch queue is a branch and the fetch unit is paused
		// then this instruction caused a branch misprediction
		bool causedMisprediction = inst.op == Instruction::BNE && i == fetchQueue.size() - 1 && !sim.instructionFetchUnit.isFetchEnabled();

		// Register references that are not applicable should already be set X0
		DecodedInstruction decoded = {
			.address = inst.address,
			.op = inst.op,
			.dest = static_cast<uint8_t>(renameDestinationRegister(inst.dest)),
			.src1 = static_cast<uint8_t>(resolveSourceRegister(inst.src1)),
			.src2 = static_cast<uint8_t>(resolveSourceRegister(inst.src2)),
			.imm = causedMisprediction ? Instruction::MISPREDICTION_SENTINEL : inst.imm,
			.archDest = inst.dest};

		sim.instructionQueue.push_back(decoded);
	}

	sim.instructionFetchUnit.consumeFetchQueue(i);
}
