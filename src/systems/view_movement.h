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

    void update(double delta_time);

private:
    sf::View *m_view;
    float m_velocity_mul;
};

} // namespace fck::system

#endif // VIEWMOVEMENT_JPQAGUYFBKPG_H
