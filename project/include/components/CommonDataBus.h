#ifndef COMPONENTS_COMMONDATABUS_H
#define COMPONENTS_COMMONDATABUS_H

class Simulator;

class CommonDataBus
{
public:
    CommonDataBus(Simulator &sim);

    void writeBack();

private:
    Simulator &simulator;
};

#endif // COMPONENTS_COMMONDATABUS_H
