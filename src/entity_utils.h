#ifndef ENTITYUTILS_RSJIGMNUAIGD_H
#define ENTITYUTILS_RSJIGMNUAIGD_H

#include "components/components.h"

#include "fck/entity.h"
#include "fck/sigslot.h"

#include <list>

namespace fck
{

struct entity
{
    static std::vector<std::string> getDrawableStates(const Entity &entity);

    // transform
    static Signal<const Entity &, const sf::Vector2f &> move;
    static Signal<const Entity &, const sf::Vector2f &> set_position;
    static Signal<const Entity &, const Entity &> set_parent;

    // state
    static Signal<const Entity &, entity_state::State> set_state;
    static Signal<const Entity &, entity_state::Direction> set_direction;

    // target
    static Signal<const Entity &, const Entity &> set_target;

    // marker
    static Signal<const Entity &, const Entity &> set_marker;

    // stats
    static Signal<const Entity &, float> set_heath;
    static Signal<const Entity &, float> set_armor;

    // destroy
    static Signal<const Entity &> destroy;

    // collided
    static Signal<const Entity &, const Entity &> collided;

    // drawable
    static Signal<const Entity &, const std::string &> set_drawable_state;

    // sounds
    static Signal<const Entity &, const std::string &> play_sound;
    static Signal<const Entity &, const std::string &> stop_sound;
    static Signal<const Entity &> stop_all_sound;

    // skills
    static Signal<const Entity &, SkillBase *> skill_applied;
    static Signal<const Entity &, SkillBase *> skill_finished;
};

} // namespace fck

#endif // ENTITYUTILS_RSJIGMNUAIGD_H
