#ifndef SCRIPT_DAWQCBFULHWB_H
#define SCRIPT_DAWQCBFULHWB_H

#include "../components/components.h"
#include "../fck/system.h"
#include "../map/map.h"

#include <sol/sol.hpp>

namespace fck::system
{

class Script : public System<component::Script>
{
public:
    Script(sol::state *sol_state);
    ~Script() = default;

    void update(double delta_time);

public: // slots
    // entity
    void onEntityEnabled(const Entity &entity);
    void onEntityDisabled(const Entity &entity);
    void onEntityDestroyed(const Entity &entity);

    void onEntityMoved(const Entity &entity, const sf::Vector2f &offset);

    void onEntityStateChanged(const Entity &entity, entity_state::State state);
    void onEntityDirectionChanged(const Entity &entity, entity_state::Direction direction);

    void onEntityCollided(const Entity &entity, const Entity &other);

    // map
    void onMapChanged(map::Map *map);

private:
    sol::state *m_sol_state;
};

} // namespace fck::system

#endif // SCRIPT_DAWQCBFULHWB_H
