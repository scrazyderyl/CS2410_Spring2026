#ifndef TYPES_BHTENTRY_H
#define TYPES_BHTENTRY_H

#include <cstdint>

struct BHTEntry
{
    uint8_t counter = 2;

    bool getPrediction() const
    {
        return counter >= 2;
    }

    void update(bool taken)
    {
        if (taken)
        {
            if (counter < 3)
            {
                counter++;
            }
        }
        else
        {
            if (counter > 0)
            {
                counter--;
            }
        }
    }
};

#endif // TYPES_BHTENTRY_H
