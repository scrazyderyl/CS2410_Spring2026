
#include <climits>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sys/types.h>
#include <vector>
#include <string>

#include "Config.h"
#include "json.hpp"
using namespace nlohmann::literals;

#include "components/InstructionDecodeUnit.h"
#include "components/InstructionDispatcher.h"
#include "components/InstructionFetchUnit.h"
#include "components/ReorderBuffer.h"
#include "components/BranchPredictor.h"
#include "components/CommonDataBus.h"
#include "components/functional_units/IntegerUnit.h"
#include "components/functional_units/LoadStoreUnit.h"
#include "components/functional_units/FPAddUnit.h"
#include "components/functional_units/FPMultUnit.h"
#include "components/functional_units/FPDivUnit.h"
#include "components/functional_units/BranchUnit.h"

#include "types/ReservationStation.h"
#include "types/ArchitecturalRegister.h"
#include "types/RegisterFileEntry.h"
#include "types/Instruction.h"

#ifndef SIMULATOR_H
#define SIMULATOR_H

#define MAX_MEM_SIZE 1024
#define NUM_PHYS_REG 32
#define NUM_PHYS_REG_INCLUDING_X0 NUM_PHYS_REG + 1 // We will use the last register as a "dummy" register to represent the value of X0, which is always 0

enum DebugArg
{
	DEBUG_DCACHE,
	DEBUG_REGISTERS
};

class Simulator
{
public:
	/**
	 * @brief Simulator Configuration
	 */
	Config *configuration;

	// Statistics
	int cc = 0;
	int rs_stalls = 0;
	int rob_stalls = 0;

	/**
	 * @brief Data Memory ($D Cache)
	 * On startup, all values are initialized to 0
	 */
	double dataMemory[MAX_MEM_SIZE] = {0};

	/**
	 * @brief Instruction cache
	 */
	std::vector<Instruction> programInstructions;

	/**
	 * @brief Register File
	 * @note This is a map of physical register number -> value
	 * @note For bookkeeping, we also store the ROB number and whether the register is busy
	 */
	RegisterFileEntry registerFile[NUM_PHYS_REG_INCLUDING_X0];
	std::map<ArchitecturalRegister, int> registerMapTable;

	BranchPredictor branchPredictor;
	InstructionFetchUnit *instructionFetchUnit;
	InstructionDecodeUnit *instructionDecodeUnit;
	InstructionDispatcher *instructionDispatcher;
	ReorderBuffer *reorderBuffer;
	CommonDataBus *cdb;

	// Functional units
	IntegerUnit *intUnit;
	LoadStoreUnit *loadStoreUnit;
	FPAddUnit *fpAddUnit;
	FPMultUnit *fpMultUnit;
	FPDivUnit *fpDivUnit;
	BranchUnit *branchUnit;

	// *PUBLIC --------------------
	/**
	 * @brief Constructor for Simulator that uses custom configuration
	 * @param program Pointer to ifstream object containing the program
	 * @param c Configuration struct
	 */
	Simulator(std::ifstream *program, Config *c);
	~Simulator();

	/**
	 * @brief Runs the simulator until the program is complete
	 */
	void runUntilCompletion();

	/**
	 * @brief Runs one cycle of the simulator
	 * @return true if the program is still running, false if the program is done
	 */
	bool runOneCycle();

	/**
	 * @brief Debugging function
	 * @param argc Number of arguments excluding argc
	 * @param ... Variable arguments - see enum DebugArg
	 *
	 * @example this->dump(1, DEBUG_ALL);
	 */
	void dump(int argc...);
	void printStats();

	/**
	 * @brief Serialize the simulator state to a JSON file
	 * @param output Pointer to ofstream object where the serialized data is written
	 */
	void serializeJSON(std::ofstream *output);
	
	void fetchStage();
	void decodeStage();
	void dispatchStage();
	void executeStage();
	void writeBackStage();
	void commitStage();
};

#endif // SIMULATOR_H
