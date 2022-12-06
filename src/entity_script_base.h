#ifndef ENTITYSCRIPTBASE_H
#define ENTITYSCRIPTBASE_H

#include "fck_common.h"

#include "fck/a_star.h"
#include "fck/b2_dynamic_tree.h"
#include "fck/entity.h"
#include "fck/world.h"

namespace fck
{

class ScriptSystem;

class EntityScriptBase
{
    friend class ScriptSystem;

public:
    EntityScriptBase();
    virtual ~EntityScriptBase() = default;

    virtual void update(const Entity &entity, double delta_time) = 0;

    virtual void onEntityEnabled(const Entity &entity);
    virtual void onEntityDisabled(const Entity &entity);
    virtual void onEntityDestroyed(const Entity &entity);

    virtual void onEntityMoved(const Entity &entity, const sf::Vector2f &offset);

    virtual void onEntityStateChanged(const Entity &entity, entity_state::State state);
    virtual void onEntityDirectionChanged(const Entity &entity, entity_state::Direction direction);

    virtual void onEntityCollided(const Entity &entity, const Entity &other);
};

} // namespace fck

#endif // ENTITYSCRIPTBASE_H
