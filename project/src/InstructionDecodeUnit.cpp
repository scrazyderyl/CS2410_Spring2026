#include "InstructionDecodeUnit.h"
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

		// Resolve sources against the previous architectural mapping before
		// assigning the new destination physical register for this instruction.
		DecodedInstruction decoded;
		decoded.address = inst.address;
		decoded.op = inst.op;
		decoded.src1 = static_cast<uint8_t>(resolveSourceRegister(inst.src1));
		decoded.src2 = static_cast<uint8_t>(resolveSourceRegister(inst.src2));
		
		// Save the old physical register index for the destination register and free it when the instruction commits
		if (!(inst.dest.type == ArchitecturalRegister::X && inst.dest.num == 0))
		{
			auto existingDest = sim.registerMapTable.find(inst.dest);

			if (existingDest != sim.registerMapTable.end())
			{
				decoded.oldPhysRegIndex = existingDest->second;
			}
		}

		decoded.dest = static_cast<uint8_t>(renameDestinationRegister(inst.dest));
		decoded.imm = causedMisprediction ? Instruction::MISPREDICTION_SENTINEL : inst.imm;
		decoded.archDest = inst.dest;

		sim.instructionQueue.push_back(decoded);
	}

	sim.instructionFetchUnit.consumeFetchQueue(i);
}
