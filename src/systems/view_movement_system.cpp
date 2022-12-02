#include "view_movement_system.h"

#include "../fck/utilities.h"

#include <spdlog/spdlog.h>

namespace fck
{

ViewMovementSystem::ViewMovementSystem() : m_view{nullptr}, m_velocity_mul{5}
{
}

void ViewMovementSystem::setView(sf::View *newView)
{
    m_view = newView;
}

void ViewMovementSystem::update(double delta_time)
{
    each<TransformComponent>(
        [this, delta_time](Entity &entity, TransformComponent &transform_component) {
            sf::Vector2f view_position = m_view->getCenter();
            sf::Vector2f entity_position = transform_component.transform.getPosition();

            float angle = vector2::angleTo(view_position, entity_position);
            float dist = vector2::distance(view_position, entity_position);

            sf::Vector2f velocity(
                -dist * m_velocity_mul * delta_time * std::cos(angle),
                -dist * m_velocity_mul * delta_time * std::sin(angle));

            if (dist > 2)
                m_view->move(velocity);
        });
}

} // namespace fck
