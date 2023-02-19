#include "movement.h"

#include "../entity_funcs.h"
#include "../fck/common.h"
#include "../fck/utilities.h"

#include "spdlog/spdlog.h"

namespace fck::system
{

Movement::Movement()
{
}

void Movement::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::Velocity &velocity_component = entity.get<component::Velocity>();

        if (vector2::isValid(velocity_component.velocity))
        {
            sf::Vector2f result_velocity = velocity_component.velocity * float(delta_time);
            entity_funcs::move(entity, result_velocity);
        }
    }
}

void Movement::onEntityStateChanged(const Entity &entity, entity_state::State state)
{
    if (!entity.has<component::Velocity>())
        return;

    if (state == entity_state::DEATH)
    {
        auto &velocity_component = entity.get<component::Velocity>();
        velocity_component.velocity = {};
    }
}

} // namespace fck::system
