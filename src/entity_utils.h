#ifndef ENTITYUTILS_H
#define ENTITYUTILS_H

#include "components/components.h"

#include "fck/entity.h"
#include "fck/sigslot.h"

#include <list>

namespace fck
{

struct EntityUtils
{
public:
    // transform
    static void moveEntity(const Entity &entity, const sf::Vector2f &offset);
    static void setEntityPosition(const Entity &entity, const sf::Vector2f &position);
    static void setEntityParent(const Entity &entity, const Entity &parent);

    // scene
    static std::list<Entity> lookAround(const Entity &entity, const sf::FloatRect &around_bounds);

    // target
    static void setEntityTarget(
        const Entity &entity, const Entity &target, const Entity &target_mark = Entity{});

    // state
    static void setEntityState(const Entity &entity, entity_state::State state);
    static void setEntityDirection(const Entity &entity, entity_state::Direction direction);

    // stats
    static void addEntityHealth(const Entity &entity, float health);
    static void setEntityHealth(const Entity &entity, float health);
    static void addEntityArmor(const Entity &entity, float armor);
    static void setEntityArmor(const Entity &entity, float armor);

public: //signals
    // transform
    static Signal<const Entity &, const sf::Vector2f &> entity_moved;
    static Signal<const Entity &, const Entity &> entity_parent_changed;

    // state
    static Signal<const Entity &, entity_state::State> entity_state_changed;
    static Signal<const Entity &, entity_state::Direction> entity_direction_changed;

    // target
    static Signal<const Entity &, const Entity &> entity_target_changed;

    // stats
    static Signal<const Entity &, float> entity_health_changed;
    static Signal<const Entity &, float> entity_armor_changed;
};

} // namespace fck

#endif // ENTITYUTILS_H
