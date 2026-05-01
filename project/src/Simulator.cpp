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

	load_program(this, program);
}

void Simulator::printStats()
{
	std::cout << "Total CCs: " << "0" << std::endl;
	std::cout << "RS Stalls: " << "0" << std::endl;
	std::cout << "ROB Stalls: " << "0" << std::endl;
};

void Simulator::serializeJSON(std::ofstream* output)
{
	nlohmann::json j;
	j["total_cycles"] = 0;
	j["rs_stalls"] = 0;
	j["rob_stalls"] = 0;
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