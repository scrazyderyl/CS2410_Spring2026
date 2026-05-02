#ifndef COMPONENTS_REORDERBUFFER_H
#define COMPONENTS_REORDERBUFFER_H

class Simulator;

class ReorderBuffer
{
public:
    ReorderBuffer(Simulator &sim);

    // Commit up to NC instructions from the ROB
    void commit();

private:
    Simulator &simulator;
};

#endif // COMPONENTS_REORDERBUFFER_H
