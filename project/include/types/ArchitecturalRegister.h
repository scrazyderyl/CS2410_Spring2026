#ifndef COMPONENTS_TYPES_ARCHITECTURALREGISTER_H
#define COMPONENTS_TYPES_ARCHITECTURALREGISTER_H

#include <cstdint>

struct ArchitecturalRegister
{
    enum
    {
        X,
        F
    } type;
    int32_t num;

    bool operator<(const ArchitecturalRegister &o) const
    {
        return (type < o.type) || (type == o.type && num < o.num);
    }

    std::string toString() const
    {
        return (type == X ? "X" : "F") + std::to_string(num);
    }
};

#endif // COMPONENTS_TYPES_ARCHITECTURALREGISTER_H
