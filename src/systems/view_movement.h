#ifndef VIEWMOVEMENT_JPQAGUYFBKPG_H
#define VIEWMOVEMENT_JPQAGUYFBKPG_H

#include "../components/components.h"

#include "../fck/system.h"
#include "../fck_common.h"

#include <SFML/Graphics.hpp>

namespace fck::system
{

class ViewMovement : public System<component::Player, component::Transform>
{
public:
    ViewMovement();
    ~ViewMovement() = default;

    void setView(sf::View *newView);
    void setMovementBounds(const sf::FloatRect &movement_bounds);

    void update(double delta_time);

private:
    sf::View *m_view;
    sf::Vector2f m_movement_lower_bound;
    sf::Vector2f m_movement_upper_bound;
    float m_velocity_mul;
};

} // namespace fck::system

#endif // VIEWMOVEMENT_JPQAGUYFBKPG_H
