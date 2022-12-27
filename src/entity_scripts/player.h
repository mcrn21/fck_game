#ifndef PLAYER_BGOAZAORIAYM_H
#define PLAYER_BGOAZAORIAYM_H

#include "entity_script_base.h"

#include <SFML/Graphics.hpp>

namespace fck::entity_script
{

class Player : public EntityScriptBase
{
public:
    Player();
    ~Player() = default;

    void update(const Entity &entity, double delta_time);

    void onEntityCollided(const Entity &entity, const Entity &other);

private:
    void updateTarget(const Entity &entity);

    void updateAroundBounds();
    void updateAroundEntities();

private:
    float m_look_distance;
    sf::FloatRect m_around_bounds;
    sf::FloatRect m_look_at_bounds;
};

} // namespace fck::entity_script

#endif // PLAYERSCRIPT_BGOAZAORIAYM_H
