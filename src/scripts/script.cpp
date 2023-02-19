#include "script.h"

#include <spdlog/spdlog.h>

namespace fck::script
{

Script::Script(const sol::table &script_table) : m_script_table{script_table}
{
    m_update_function = m_script_table["update"];

    m_entity_enabled_function = m_script_table["onEntityEnabled"];
    m_entity_disabled_function = m_script_table["onEntityDisabled"];
    m_entity_destroyed_function = m_script_table["onEntityDestroyed"];

    m_entity_moved_function = m_script_table["onEntityMoved"];

    m_entity_state_changed_function = m_script_table["onEntityStateChanged"];
    m_entity_direction_changed_function = m_script_table["onEntityDirectionChanged"];

    m_entity_collided_function = m_script_table["onEntityCollided"];
}

sol::table &Script::getScriptTable()
{
    return m_script_table;
}

void Script::setEntityToTable(const Entity &entity)
{
    m_script_table["entity"] = entity;
}

void Script::update(double delta_time)
{
    if (m_update_function)
    {
        auto result = m_update_function(m_script_table, delta_time);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityDestroyed: {}", err.what());
        }
    }
}

void Script::onEntityEnabled()
{
    if (m_entity_enabled_function)
    {
        auto result = m_entity_enabled_function(m_script_table);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityDestroyed: {}", err.what());
        }
    }
}

void Script::onEntityDisabled()
{
    if (m_entity_disabled_function)
    {
        auto result = m_entity_disabled_function(m_script_table);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityDestroyed: {}", err.what());
        }
    }
}

void Script::onEntityDestroyed()
{
    if (m_entity_destroyed_function)
    {
        auto result = m_entity_destroyed_function(m_script_table);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityDestroyed: {}", err.what());
        }
    }
}

void Script::onEntityMoved(const sf::Vector2f &offset)
{
    if (m_entity_moved_function)
    {
        auto result = m_entity_moved_function(m_script_table, offset);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityMoved: {}", err.what());
        }
    }
}

void Script::onEntityStateChanged(entity_state::State state)
{
    if (m_entity_state_changed_function)
    {
        auto result = m_entity_state_changed_function(m_script_table, state);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityStateChanged: {}", err.what());
        }
    }
}

void Script::onEntityDirectionChanged(entity_state::Direction direction)
{
    if (m_entity_direction_changed_function)
    {
        auto result = m_entity_direction_changed_function(m_script_table, direction);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityDirectionChanged: {}", err.what());
        }
    }
}

void Script::onEntityCollided(const Entity &other)
{
    if (m_entity_collided_function)
    {
        auto result = m_entity_collided_function(m_script_table, other);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Script::onEntityCollided: {}", err.what());
        }
    }
}

} // namespace fck::script
