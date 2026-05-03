#ifndef COMPONENTS_BRANCHPREDICTOR_H
#define COMPONENTS_BRANCHPREDICTOR_H

#include <vector>
#include <map>
#include <types/BHTEntry.h>
#include <types/BTBEntry.h>
#include <types/ArchitecturalRegisterFile.h>
#include <types/Instruction.h>

// Since the fetcher is checking against the actual outcome, the branch predictor isn't really needed by the simulator
// It just needs the outcome and can derive the target address from the instruction itself
// Which I realized immediately after writing all the code
class BranchPredictor
{
public:
    static const int BTB_SIZE = 16;
    static const int BTB_INDEX_SHIFT = 4;

    BranchPredictor(const std::vector<Instruction> &programInstructions, const double *initialMemory);

    // Simple simulator to get the actual branch outcomes ahead of time
    // I guess if you really wanted you could run the actual simulator and record the outcomes but this is much faster to run
    static std::vector<bool> simulateBranches(const std::vector<Instruction> &programInstructions, const double *initialMemory);

    // This should only be called once for every bne instruction
    bool isMispredicted(bool prediction);

    bool predict(int branchAddress);

    int getTargetAddress(int branchAddress);

    void update(int branchAddress, bool taken);


private:
    // Readme doesn't specify the size of the BHT so using the same size as the BTB
    BHTEntry bht[BTB_SIZE];
    BTBEntry btb[BTB_SIZE];
    
    // The oracle branch predictor they told you not to worry about:
    std::vector<bool> branchOutcomes;

    int branchesEncountered = 0;

    static int hashAddress(int address)
    {
        return (address >> BTB_INDEX_SHIFT) & (BTB_SIZE - 1);
    }

};

#endif // COMPONENTS_BRANCHPREDICTOR_H
