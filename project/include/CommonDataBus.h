#ifndef COMPONENTS_COMMONDATABUS_H
#define COMPONENTS_COMMONDATABUS_H

class Simulator;

class CommonDataBus
{
public:
    static void writeBack(Simulator &sim);
};

#endif // COMPONENTS_COMMONDATABUS_H
