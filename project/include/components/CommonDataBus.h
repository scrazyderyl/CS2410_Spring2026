#ifndef COMPONENTS_COMMONDATABUS_H
#define COMPONENTS_COMMONDATABUS_H

class Simulator;

class CommonDataBus
{
public:
    static void writeBack(Simulator &simulator);
};

#endif // COMPONENTS_COMMONDATABUS_H
