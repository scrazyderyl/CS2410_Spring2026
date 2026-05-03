#include "components/InstructionDecodeUnit.h"
#include "Simulator.h"

#include <stdint.h>

InstructionDecodeUnit::InstructionDecodeUnit(Simulator &sim) : simulator(sim) {}

int InstructionDecodeUnit::allocatePhysicalRegister()
{
	if (simulator.freePhysicalRegisters.empty())
	{
		return -1;
	}

	int physReg = simulator.freePhysicalRegisters.front();
	simulator.freePhysicalRegisters.pop();

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
	auto existing = simulator.registerMapTable.find(reg);

	if (existing != simulator.registerMapTable.end())
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

	simulator.registerFile[physReg].value = simulator.architecturalRegisterFile.getValue(reg);
	simulator.registerMapTable[reg] = physReg;

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

	simulator.registerMapTable[reg] = physReg;

	return physReg;
}

void InstructionDecodeUnit::decode()
{
	const std::vector<Instruction> &fetchQueue = simulator.instructionFetchUnit.getFetchQueue();

	size_t i = 0;

	for (; i < fetchQueue.size() && static_cast<int>(simulator.instructionQueue.size()) < simulator.configuration->NI; i++)
	{
		const Instruction &inst = fetchQueue[i];

		// Register fields that are not applicable should already be set X0
		DecodedInstruction decoded = {
			.op = inst.op,
			.dest = static_cast<uint8_t>(renameDestinationRegister(inst.dest)),
			.src1 = static_cast<uint8_t>(resolveSourceRegister(inst.src1)),
			.src2 = static_cast<uint8_t>(resolveSourceRegister(inst.src2)),
			.imm = inst.imm,
			.archDest = inst.dest
		};

		simulator.instructionQueue.push_back(decoded);
	}

	simulator.instructionFetchUnit.consumeFetchQueue(i);
}
