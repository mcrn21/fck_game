#ifndef PLAYERSCRIPT_H
#define PLAYERSCRIPT_H

#include "../entity_script_base.h"

#include <list>

namespace fck
{

class PlayerScript : public EntityScriptBase
{
public:
    PlayerScript();
    ~PlayerScript() = default;

    void update(const Entity &entity, double delta_time);

private:
    void updateTarget(const Entity &entity);

    void updateAroundBounds();
    void updateAroundEntities();

private:
    float m_look_distance;
    sf::FloatRect m_around_bounds;
    sf::FloatRect m_look_at_bounds;

    std::list<Entity> m_around_entities;
    std::list<Entity> m_look_at_entities;
};

} // namespace fck

#endif // PLAYERSCRIPT_H
