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

void ViewMovement::setMovementBounds(const sf::FloatRect &movement_bounds)
{
    m_movement_lower_bound = movement_bounds.getPosition();
    m_movement_upper_bound = m_movement_lower_bound + movement_bounds.getSize();
}

void ViewMovement::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::Transform &transform_component = entity.getComponent<component::Transform>();
        component::Player &player_component = entity.getComponent<component::Player>();

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

            if (m_movement_upper_bound.x > 0 && m_movement_upper_bound.y > 0)
            {
                sf::Vector2f offset;
                sf::Vector2f lower_point = m_view->getCenter() - m_view->getSize() / 2.0f;
                sf::Vector2f upper_point = lower_point + m_view->getSize();

                if (lower_point.x < m_movement_lower_bound.x)
                    offset.x -= (lower_point.x - m_movement_lower_bound.x);

                if (lower_point.y < m_movement_lower_bound.y)
                    offset.y -= (lower_point.y - m_movement_lower_bound.y);

                if (upper_point.x > (m_movement_upper_bound.x))
                    offset.x -= (upper_point.x - m_movement_upper_bound.x);

                if (upper_point.y > (m_movement_upper_bound.y))
                    offset.y -= (upper_point.y - m_movement_upper_bound.y);

                if (offset.x != 0 || offset.y != 0)
                    m_view->move(offset);
            }
        }
    }
}

} // namespace fck::system
