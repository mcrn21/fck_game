#ifndef STATSSYSTEM_H
#define STATSSYSTEM_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck
{

class StatsSystem : public System<StatsComponent, StateComponent>
{
public:
    StatsSystem();
    ~StatsSystem() = default;

    void update(double delta_time);
};

} // namespace fck

#endif // STATSSYSTEM_H
