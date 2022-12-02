#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck
{

class MovementSystem : public System<VelocityComponent>
{
public:
    MovementSystem();
    ~MovementSystem() = default;

    void update(double delta_time);
};

} // namespace fck

#endif // MOVEMENTSYSTEM_H
