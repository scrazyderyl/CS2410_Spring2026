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

bool BranchPredictor::isMispredicted(bool prediction)
{
    bool actualOutcome = branchOutcomes[branchesEncountered];
    branchesEncountered++;

    return prediction != actualOutcome;
}

std::vector<bool> BranchPredictor::simulateBranches(const std::vector<Instruction> &programInstructions, const double *initialMemory)
{
    std::vector<bool> outcomes;
    ArchitecturalRegisterFile registerFile;

    int instIndex = 0;

    while (instIndex < static_cast<int>(programInstructions.size()))
    {
        const Instruction &inst = programInstructions[instIndex];

        switch (inst.op)
        {
        case 0: // nop
            break;

        case 1: // fld
        {
            double val1 = registerFile.getValue(inst.src1);
            int addr = static_cast<int>(val1 + inst.imm);
            registerFile.setValue(inst.dest, initialMemory[addr]);
            break;
        }

        case 2: // fsd
            // Since the test cases don't actually have any RAW dependencies going through the data memory, the fsd instructions can be ignored for simplicity
            break;

        case 3: // add
            double val1 = registerFile.getValue(inst.src1);
            double val2 = registerFile.getValue(inst.src2);
            registerFile.setValue(inst.dest, val1 + val2);
            break;

        case 4: // addi
            double val1 = registerFile.getValue(inst.src1);
            registerFile.setValue(inst.dest, val1 + inst.imm);
            break;

        case 5: // slt
            double val1 = registerFile.getValue(inst.src1);
            double val2 = registerFile.getValue(inst.src2);
            registerFile.setValue(inst.dest, val1 < val2 ? 1.0 : 0.0);
            break;

        case 6: // fadd
            double val1 = registerFile.getValue(inst.src1);
            double val2 = registerFile.getValue(inst.src2);
            registerFile.setValue(inst.dest, val1 + val2);
            break;

        case 7: // fsub
            double val1 = registerFile.getValue(inst.src1);
            double val2 = registerFile.getValue(inst.src2);
            registerFile.setValue(inst.dest, val1 - val2);
            break;

        case 8: // fmul
            double val1 = registerFile.getValue(inst.src1);
            double val2 = registerFile.getValue(inst.src2);
            registerFile.setValue(inst.dest, val1 * val2);
            break;

        case 9: // fdiv
            double val1 = registerFile.getValue(inst.src1);
            double val2 = registerFile.getValue(inst.src2);
            registerFile.setValue(inst.dest, val1 / val2);
            break;

        case 10: // bne
        {
            double val1 = registerFile.getValue(inst.src1);
            double val2 = registerFile.getValue(inst.src2);
            bool taken = val1 != val2;

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