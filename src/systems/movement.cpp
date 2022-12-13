#include "movement.h"

#include "../entity_utils.h"

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
        component::Velocity &velocity_component
            = entity.getComponent<component::Velocity>();

        if (vector2::isValid(velocity_component.velocity))
        {
            sf::Vector2f result_velocity = velocity_component.velocity * float(delta_time);
            entity::move.emit(entity, result_velocity);
        }
    }
}

} // namespace fck::system
