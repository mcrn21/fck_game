#include "script.h"

#include "spdlog/spdlog.h"

namespace fck::system
{

Script::Script(sol::state *sol_state) : m_sol_state{sol_state}
{
}

void Script::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        auto &script_component = entity.get<component::Script>();
        if (script_component.script)
            script_component.script->update(delta_time);
    }
}

void Script::onEntityEnabled(const Entity &entity)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();
    if (script_component.script)
        script_component.script->onEntityEnabled();
}

void Script::onEntityDisabled(const Entity &entity)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();
    if (script_component.script)
        script_component.script->onEntityDisabled();
}

void Script::onEntityDestroyed(const Entity &entity)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();
    if (script_component.script)
        script_component.script->onEntityDestroyed();
}

void Script::onEntityMoved(const Entity &entity, const sf::Vector2f &offset)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();
    if (script_component.script)
        script_component.script->onEntityMoved(offset);
}

void Script::onEntityStateChanged(const Entity &entity, entity_state::State state)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();
    if (script_component.script)
        script_component.script->onEntityStateChanged(state);
}

void Script::onEntityDirectionChanged(const Entity &entity, entity_state::Direction direction)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();
    if (script_component.script)
        script_component.script->onEntityDirectionChanged(direction);
}

void Script::onEntityCollided(const Entity &entity, const Entity &other)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();
    if (script_component.script)
        script_component.script->onEntityCollided(other);
}

void Script::onMapChanged(map::Map *map)
{
    sol::table fck_namespace = (*m_sol_state)["fck"].get_or_create<sol::table>();
    sol::table map_namespace = fck_namespace["map"].get_or_create<sol::table>();

    map_namespace["map"] = map;
}

} // namespace fck::system
