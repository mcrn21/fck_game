#ifndef VIEWMOVEMENTSYSTEM_H
#define VIEWMOVEMENTSYSTEM_H

#include "../components/components.h"

#include "../fck/system.h"
#include "../fck_common.h"

#include <SFML/Graphics.hpp>

namespace fck
{

class ViewMovementSystem : public System<PlayerComponent, TransformComponent>
{
public:
    ViewMovementSystem();
    ~ViewMovementSystem() = default;

    void setView(sf::View *newView);

    void update(double delta_time);

private:
    sf::View *m_view;
    float m_velocity_mul;
};

} // namespace fck

#endif // VIEWMOVEMENTSYSTEM_H
