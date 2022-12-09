#include "view_movement.h"

#include "../fck/utilities.h"

#include <spdlog/spdlog.h>

namespace fck::system
{

ViewMovement::ViewMovement() : m_view{nullptr}, m_velocity_mul{5}
{
}

void ViewMovement::setView(sf::View *newView)
{
    m_view = newView;
}

void ViewMovement::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        component::Transform &transform_component
            = entity.component<component::Transform>();
        component::Player &player_component = entity.component<component::Player>();

        if (player_component.view_hard_set_position)
        {
            m_view->setCenter(transform_component.transform.getPosition());
            player_component.view_hard_set_position = false;
        }
        else
        {
            sf::Vector2f view_position = m_view->getCenter();
            sf::Vector2f entity_position = transform_component.transform.getPosition();

            float angle = vector2::angleTo(view_position, entity_position);
            float dist = vector2::distance(view_position, entity_position);

            sf::Vector2f velocity(
                -dist * m_velocity_mul * delta_time * std::cos(angle),
                -dist * m_velocity_mul * delta_time * std::sin(angle));

            if (dist > 2)
                m_view->move(velocity);
        }
    }
}

} // namespace fck::system
