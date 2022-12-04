#ifndef ENTITYUTILS_H
#define ENTITYUTILS_H

#include "components/components.h"

#include "fck/entity.h"
#include "fck/sigslot.h"

#include <list>

namespace fck
{

struct entity
{
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
};

} // namespace fck

#endif // ENTITYUTILS_H
