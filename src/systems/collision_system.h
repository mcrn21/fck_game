#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck
{

class CollisionSystem
    : public System<SceneComponent, CollisionComponent, VelocityComponent, TransformComponent>
{
public:
    CollisionSystem();
    ~CollisionSystem() = default;

    void update(double delta_time);
};

} // namespace fck

#endif // COLLISIONSYSTEM_H
