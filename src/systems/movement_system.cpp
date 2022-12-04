#include "movement_system.h"

#include "../entity_utils.h"

#include "../fck/common.h"
#include "../fck/utilities.h"

#include "spdlog/spdlog.h"

namespace fck
{

MovementSystem::MovementSystem()
{
}

void MovementSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        VelocityComponent &velocity_component = entity.component<VelocityComponent>();

        if (vector2::isValid(velocity_component.velocity))
        {
            sf::Vector2f result_velocity = velocity_component.velocity * float(delta_time);
            entity::move.emit(entity, result_velocity);
        }
    }
}

} // namespace fck
