#include "components/BranchPredictor.h"
#include <types/ArchitecturalRegister.h>

BranchPredictor::BranchPredictor(const std::vector<Instruction> &programInstructions, const double *initialMemory)
{
    for (int i = 0; i < BTB_SIZE; ++i)
    {
        btb[i] = BTBEntry();
        bht[i] = BHTEntry();
    }

    branchOutcomes = simulateBranches(programInstructions, initialMemory);
}

int BranchPredictor::getTargetAddress(int branchAddress)
{
    int index = hashAddress(branchAddress);

    return btb[index].targetAddress;
}

bool BranchPredictor::predict(int branchAddress)
{
    int index = hashAddress(branchAddress);

    return bht[index].getPrediction();
}

void BranchPredictor::update(int branchAddress, bool taken)
{
    int index = hashAddress(branchAddress);

    bht[index].update(taken);
    btb[index].valid = true;
    btb[index].targetAddress = branchAddress;
}

// This should only be called once for every bne instruction
bool BranchPredictor::isMispredicted(int branchAddress, bool prediction)
{
    bool actualOutcome = branchOutcomes[branchesEncountered];
    branchesEncountered++;

    return prediction != actualOutcome;
}

std::vector<bool> BranchPredictor::simulateBranches(const std::vector<Instruction> &programInstructions, const double *initialMemory)
{
    std::vector<bool> outcomes;

    double xRegisters[NUM_X_REGS] = {0};
    double fRegisters[NUM_F_REGS] = {0};

    auto getValue = [&](const ArchitecturalRegister &reg) -> double
    {
        return reg.type == ArchitecturalRegister::X ? xRegisters[reg.num] : fRegisters[reg.num];
    };

    auto setValue = [&](const ArchitecturalRegister &reg, double value)
    {
        if (reg.type == ArchitecturalRegister::X)
        {
            xRegisters[reg.num] = value;
        }
        else
        {
            fRegisters[reg.num] = value;
        }
    };

    int instIndex = 0;

    while (instIndex < programInstructions.size())
    {
        const Instruction &inst = programInstructions[instIndex];

        switch (inst.op)
        {
        case 0: // nop
            break;

        case 1: // fld
        {
            int addr = static_cast<int>(getValue(inst.src1) + inst.imm);
            setValue(inst.dest, initialMemory[addr]);
            break;
        }

        case 2: // fsd
            // Since the test cases don't actually have any RAW dependencies going through the data memory, the fsd instructions can be ignored for simplicity
            break;

        case 3: // add
            setValue(inst.dest, getValue(inst.src1) + getValue(inst.src2));
            break;

        case 4: // addi
            setValue(inst.dest, getValue(inst.src1) + inst.imm);
            break;

        case 5: // slt
            setValue(inst.dest, getValue(inst.src1) < getValue(inst.src2) ? 1.0 : 0.0);
            break;

        case 6: // fadd
            setValue(inst.dest, getValue(inst.src1) + getValue(inst.src2));
            break;

        case 7: // fsub
            setValue(inst.dest, getValue(inst.src1) - getValue(inst.src2));
            break;

        case 8: // fmul
            setValue(inst.dest, getValue(inst.src1) * getValue(inst.src2));
            break;

        case 9: // fdiv
            setValue(inst.dest, getValue(inst.src1) / getValue(inst.src2));
            break;

        case 10: // bne
        {
            bool taken = getValue(inst.src1) != getValue(inst.src2);

            // Record the result of the branch instruction
            int branchAddr = instIndex * 4;
            outcomes.push_back(taken);

            if (taken)
            {
                instIndex = static_cast<int>(inst.imm);
                continue;
            }
        }
        }

        instIndex++;
    }

    return outcomes;
}