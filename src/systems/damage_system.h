#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck
{

class DamageSystem : public System<DamageComponent, StatsComponent>
{
public:
    DamageSystem();
    ~DamageSystem() = default;

    void update(double delta_time);
};

} // namespace fck

#endif // DAMAGESYSTEM_H
