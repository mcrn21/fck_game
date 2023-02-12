#include "entity_script_base.h"

namespace fck
{

EntityScriptBase::EntityScriptBase()
{
}

void EntityScriptBase::update(const Entity &entity, double delta_time)
{

}

void EntityScriptBase::onEntityEnabled(const Entity &entity)
{

}

void EntityScriptBase::onEntityDisabled(const Entity &entity)
{

}

void EntityScriptBase::onEntityDestroyed(const Entity &entity)
{

}

void EntityScriptBase::onEntityMoved(const Entity &entity, const sf::Vector2f &offset)
{
}

void EntityScriptBase::onEntityStateChanged(const Entity &entity, entity_state::State state)
{
}

void EntityScriptBase::onEntityDirectionChanged(
    const Entity &entity, entity_state::Direction direction)
{
}

void EntityScriptBase::onEntityCollided(const Entity &entity, const Entity &other)
{

}

} // namespace fck
