#include "Simulator.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>
#include <cctype>

// Loading in programs in .dat files
void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
									{ return !std::isspace(ch); }));
}

void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
						 { return !std::isspace(ch); })
				.base(),
			s.end());
}

void trim(std::string &s)
{
	ltrim(s);
	rtrim(s);
}

void removeComment(std::string &line)
{
	// Check if line has a comment and take substring if needed
	size_t pctIndex = line.find('%');

	if (pctIndex != std::string::npos)
	{
		line = line.substr(0, pctIndex);
		trim(line);
	}
}

ArchitecturalRegister parseRegister(const std::string &regStr)
{
	switch (regStr[0])
	{
	case 'X':
		return ArchitecturalRegister{ArchitecturalRegister::X, std::stoi(regStr.substr(1))};
	case 'F':
		return ArchitecturalRegister{ArchitecturalRegister::F, std::stoi(regStr.substr(1))};
	default:
		throw std::invalid_argument("Invalid register: " + regStr);
	}
}

void parseLoadStoreInstruction(const std::string &operandsStr, Instruction &inst)
{
	size_t commaIndex = operandsStr.find(',');

	// Source or destination register
	std::string regStr = operandsStr.substr(0, commaIndex);
	trim(regStr);
	inst.dest = parseRegister(regStr);

	std::string addressStr = operandsStr.substr(commaIndex + 1);
	trim(addressStr);
	size_t parenIndex = addressStr.find('(');

	// Offset
	double offset = std::stod(addressStr.substr(0, parenIndex));
	inst.imm = offset;

	// Base register
	std::string baseRegStr = addressStr.substr(parenIndex + 1, addressStr.length() - parenIndex - 2);
	trim(baseRegStr);
	inst.src1 = parseRegister(baseRegStr);
}

void parseBranchInstruction(const std::string &operandsStr, Instruction &inst, std::map<std::string, int> &labelAddresses)
{
	size_t commaIndex = operandsStr.find(',');
	size_t secondCommaIndex = operandsStr.find(',', commaIndex + 1);

	// Register 1
	std::string rsStr = operandsStr.substr(0, commaIndex);
	trim(rsStr);
	inst.src1 = parseRegister(rsStr);

	// Register 2
	std::string rtStr = operandsStr.substr(commaIndex + 1, secondCommaIndex - commaIndex - 1);
	trim(rtStr);
	inst.src2 = parseRegister(rtStr);

	// Label
	std::string labelStr = operandsStr.substr(secondCommaIndex + 1);
	trim(labelStr);
	inst.imm = labelAddresses[labelStr];
}

void parseITypeInstruction(const std::string &operandsStr, Instruction &inst)
{
	size_t commaIndex = operandsStr.find(',');
	size_t secondCommaIndex = operandsStr.find(',', commaIndex + 1);

	// Destination register
	std::string rdStr = operandsStr.substr(0, commaIndex);
	trim(rdStr);
	inst.dest = parseRegister(rdStr);

	// Source register
	std::string rsStr = operandsStr.substr(commaIndex + 1, secondCommaIndex - commaIndex - 1);
	trim(rsStr);
	inst.src1 = parseRegister(rsStr);

	// Immediate
	std::string immStr = operandsStr.substr(secondCommaIndex + 1);
	trim(immStr);
	inst.imm = std::stod(immStr);
}

void parseRTypeInstruction(const std::string &operandsStr, Instruction &inst)
{
	size_t commaIndex = operandsStr.find(',');
	size_t secondCommaIndex = operandsStr.find(',', commaIndex + 1);

	// Destination register
	std::string rdStr = operandsStr.substr(0, commaIndex);
	trim(rdStr);
	inst.dest = parseRegister(rdStr);

	// Source register 1
	std::string rsStr = operandsStr.substr(commaIndex + 1, secondCommaIndex - commaIndex - 1);
	trim(rsStr);
	inst.src1 = parseRegister(rsStr);

	// Source register 2
	std::string rtStr = operandsStr.substr(secondCommaIndex + 1);
	trim(rtStr);
	inst.src2 = parseRegister(rtStr);
}

void load_program(Simulator *sim, std::ifstream *program)
{
	std::string line;

	// Parse data segment
	while (std::getline(*program, line))
	{
		trim(line);

		// Marker for start of code segment
		if (line == "%%")
		{
			break;
		}

		removeComment(line);

		// Empty line
		if (line.empty())
		{
			continue;
		}

		// Parse data
		size_t commaIndex = line.find(',');

		if (commaIndex != std::string::npos)
		{
			std::string addrStr = line.substr(0, commaIndex);
			trim(addrStr);

			std::string valStr = line.substr(commaIndex + 1);
			trim(valStr);

			if (!addrStr.empty() && !valStr.empty())
			{
				try
				{
					int addr = std::stoi(addrStr);
					double val = std::stod(valStr);

					if (addr >= 0 && addr < MAX_MEM_SIZE)
					{
						sim->dataMemory[addr] = val;
					}
				}
				catch (...)
				{
				}
			}
		}
	}

	std::map<std::string, int> labelAddresses;
	std::vector<std::string> programInstructions;

	// Process labels
	while (std::getline(*program, line))
	{
		removeComment(line);

		// Empty line
		if (line.empty())
		{
			continue;
		}

		// Handle labels
		size_t colonIndex = line.find(':');

		if (colonIndex != std::string::npos)
		{
			std::string label = line.substr(0, colonIndex);

			if (!label.empty())
			{
				labelAddresses[label] = (int)programInstructions.size() * 4;
			}

			std::string instruction = line.substr(colonIndex + 1);
			trim(instruction);

			programInstructions.push_back(instruction);
		}
		else
		{
			// Store unprocessed assembly instruction
			programInstructions.push_back(line);
		}
	}

	// Convert to machine code
	for (size_t i = 0; i < programInstructions.size(); i++)
	{
		std::string instruction = programInstructions[i];

		size_t spaceIndex = instruction.find(' ');

		// Process op code
		std::string opcodeStr = instruction.substr(0, spaceIndex);
		uint8_t opcode = instToOpCode.at(opcodeStr);

		Instruction inst;
		inst.op = opcode;

		std::string operandsStr = instruction.substr(spaceIndex + 1);

		// Process operands based on instruction type
		switch (opcode)
		{
		case 1: // fld
		case 2: // fsd
			parseLoadStoreInstruction(operandsStr, inst);
			break;
		case 4: // addi
			parseITypeInstruction(operandsStr, inst);
			break;
		case 10: // bne
			parseBranchInstruction(operandsStr, inst, labelAddresses);
			break;
		default: // others
			parseRTypeInstruction(operandsStr, inst);
			break;
		}

		sim->programInstructions.push_back(inst);
	}
}

Simulator::Simulator(std::ifstream *program, Config *c)
{
	configuration = c;

	instructionFetchUnit = new InstructionFetchUnit(*this);
	instructionDecodeUnit = new InstructionDecodeUnit(*this);
	instructionDispatcher = new InstructionDispatcher(*this);
	reorderBuffer = new ReorderBuffer(*this);

	// Instantiate functional units and keep pointers in simulator
	intUnit = new IntegerUnit(registerFile);
	loadStoreUnit = new LoadStoreUnit(dataMemory, registerFile);
	fpAddUnit = new FPAddUnit(registerFile);
	fpMultUnit = new FPMultUnit(registerFile);
	fpDivUnit = new FPDivUnit(registerFile);
	branchUnit = new BranchUnit(registerFile);

	instructionDispatcher->registerInstructionExecuter(0, nullptr);
	instructionDispatcher->registerInstructionExecuter(1, loadStoreUnit);
	instructionDispatcher->registerInstructionExecuter(2, loadStoreUnit);
	instructionDispatcher->registerInstructionExecuter(3, intUnit);
	instructionDispatcher->registerInstructionExecuter(4, intUnit);
	instructionDispatcher->registerInstructionExecuter(5, intUnit);
	instructionDispatcher->registerInstructionExecuter(6, fpAddUnit);
	instructionDispatcher->registerInstructionExecuter(7, fpAddUnit);
	instructionDispatcher->registerInstructionExecuter(8, fpMultUnit);
	instructionDispatcher->registerInstructionExecuter(9, fpDivUnit);
	instructionDispatcher->registerInstructionExecuter(10, branchUnit);

	load_program(this, program);

	branchPredictor = new BranchPredictor(programInstructions, dataMemory);
}

Simulator::~Simulator()
{
	delete branchPredictor;
	delete instructionFetchUnit;
	delete instructionDecodeUnit;
	delete instructionDispatcher;
	delete reorderBuffer;

	delete intUnit;
	delete loadStoreUnit;
	delete fpAddUnit;
	delete fpMultUnit;
	delete fpDivUnit;
	delete branchUnit;
}

void Simulator::runUntilCompletion()
{
	while (runOneCycle())
	{
		// Can put debug code here if needed
	}
}

bool Simulator::runOneCycle()
{
	// Commit stage: Commit up to NC instructions from the ROB
	// TODO: Implement commit stage
	commitStage();

	// Execute stage: Execute one instruction from each functional unit
	// TODO: Implement execute stage
	executeStage();

	// Write back stage: Write results to CDB (up to NB results)
	// (writeback happens after execute to simulate falling-edge writeback)
	writeBackStage();
	
	// Dispatch stage: Dispatch up to NW instructions from instruction queue to reservation stations
	// TODO: Implement dispatch stage
	dispatchStage();
	
	// Decode stage: Decode fetched instructions and store in instruction queue
	// TODO: Implement decode stage
	decodeStage();
	
	// Fetch stage: Fetch up to NF instructions from instruction cache (last, so new instructions don't execute this cycle)
	// TODO: Implement fetch stage
	fetchStage();
	
	// Increment cycle counter
	cc++;
	
	// Check if program is complete
	// TODO: Implement program completion check
	// For now, return false (program complete) to avoid infinite loop
	return false;
}

void Simulator::commitStage()
{
	reorderBuffer->commit();
}

void Simulator::executeStage()
{
	intUnit->execute();
	loadStoreUnit->execute();
	fpAddUnit->execute();
	fpMultUnit->execute();
	fpDivUnit->execute();
	branchUnit->execute();
}

void Simulator::writeBackStage()
{
	CommonDataBus::writeBack(*this);
}

void Simulator::dispatchStage()
{
	instructionDispatcher->dispatch();
}

void Simulator::decodeStage()
{
	instructionDecodeUnit->decode();
}

void Simulator::fetchStage()
{
	instructionFetchUnit->fetch();
}

void Simulator::printStats()
{
	std::cout << "Total CCs: " << cc << std::endl;
	std::cout << "RS Stalls: " << rs_stalls << std::endl;
	std::cout << "ROB Stalls: " << rob_stalls << std::endl;
};

void Simulator::serializeJSON(std::ofstream *output)
{
	nlohmann::json j;
	j["total_cycles"] = cc;
	j["rs_stalls"] = rs_stalls;
	j["rob_stalls"] = rob_stalls;
	for (int i = 0; i < MAX_MEM_SIZE; i++)
	{
		j["data_memory"][i] = dataMemory[i];
	}
	for (int i = 0; i < NUM_PHYS_REG; i++)
	{
		j["register_file"][i] = registerFile[i].value;
	}
	*output << j.dump(4) << std::endl;
}