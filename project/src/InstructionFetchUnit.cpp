#include "components/InstructionFetchUnit.h"
#include "Simulator.h"

InstructionFetchUnit::InstructionFetchUnit(Simulator &sim) : sim(sim) {}

std::vector<Instruction> InstructionFetchUnit::fetch()
{
	if (!fetchEnabled)
	{
		return fetchQueue;
	}

	// Populate the fetch queue with up to NF instructions
	while (static_cast<int>(fetchQueue.size()) < sim.configuration->NF && nextFetchPC < sim.programInstructions.size() * 4)
	{
		const Instruction &instruction = sim.programInstructions[nextFetchPC / 4];
		fetchQueue.push_back(instruction);

		// Handling for branches
		if (instruction.op == 10)
		{
			// If the branch has not been seen before, consider it mispredicted
			if (sim.branchPredictor.getTargetAddress(nextFetchPC) == -1) {
				fetchEnabled = false;
				break;
			}

            // Stop fetching if branch is mispredicted
			bool prediction = sim.branchPredictor.predict(nextFetchPC);

			if (sim.branchPredictor.isMispredicted(prediction))
			{
				fetchEnabled = false;
				break;
			}

            // Take branch if predicted taken
			if (prediction)
			{
				nextFetchPC = static_cast<unsigned int>(instruction.imm);
				continue;
			}
		}

		nextFetchPC += 4;
	}

	return fetchQueue;
}

const std::vector<Instruction> &InstructionFetchUnit::getFetchQueue()
{
	return fetchQueue;
}

bool InstructionFetchUnit::isQueueEmpty()
{
	return fetchQueue.empty();
}

void InstructionFetchUnit::consumeFetchQueue(std::size_t count)
{
	if (count >= fetchQueue.size())
	{
		fetchQueue.clear();
		return;
	}

	fetchQueue.erase(fetchQueue.begin(), fetchQueue.begin() + count);
}

void InstructionFetchUnit::setFetchEnabled(bool enabled)
{
	fetchEnabled = enabled;
}