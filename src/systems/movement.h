#ifndef MOVEMENT_QZGLSQZIJJUR_H
#define MOVEMENT_QZGLSQZIJJUR_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck::system
{

class Movement : public System<component::Velocity, component::Transform>
{
public:
    Movement();
    ~Movement() = default;

    void update(double delta_time);

public: // slots
    void onEntityStateChanged(const Entity &entity, entity_state::State state);
};

} // namespace fck::system

#endif // MOVEMENT_QZGLSQZIJJUR_H
