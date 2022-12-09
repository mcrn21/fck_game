#ifndef COLLISION_IDMVKMWAHIXC_H
#define COLLISION_IDMVKMWAHIXC_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck::system
{

class Collision : public System<
                      component::Scene,
                      component::Collision,
                      component::Velocity,
                      component::Transform>
{
public:
    Collision();
    ~Collision() = default;

    void update(double delta_time);
};

} // namespace fck::system

#endif // COLLISION_IDMVKMWAHIXC_H
