#include "components/InstructionFetchUnit.h"
#include "Simulator.h"

InstructionFetchUnit::InstructionFetchUnit(Simulator &sim) : sim(sim) {}

std::vector<Instruction> InstructionFetchUnit::fetch(uint pc)
{
	if (!fetchEnabled)
	{
		return fetchQueue;
	}

	while (static_cast<int>(fetchQueue.size()) < sim.configuration->NF && pc < sim.programInstructions.size() * 4)
	{
		const Instruction &instruction = sim.programInstructions[pc / 4];
		fetchQueue.push_back(instruction);

		pc += 4;

		if (instruction.op == 10)
		{
            // Stop fetching if branch is mispredicted
			bool prediction = sim.branchPredictor.predict(pc);

			if (sim.branchPredictor.isMispredicted(prediction))
			{
				fetchEnabled = false;
				break;
			}

            // Take branch if predicted taken
			if (prediction)
			{
				pc = static_cast<int>(instruction.imm);
			}
		}
	}

	return fetchQueue;
}

void InstructionFetchUnit::setFetchEnabled(bool enabled)
{
	fetchEnabled = enabled;
}