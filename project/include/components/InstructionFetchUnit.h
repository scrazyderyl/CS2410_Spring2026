#ifndef COMPONENTS_INSTRUCTIONFETCHUNIT_H
#define COMPONENTS_INSTRUCTIONFETCHUNIT_H

class Simulator;

class InstructionFetchUnit
{
public:
    InstructionFetchUnit(Simulator &sim);

    // Fetch up to NF instructions from instruction cache
    void fetch();

private:
    Simulator &simulator;
};

#endif // COMPONENTS_INSTRUCTIONFETCHUNIT_H
