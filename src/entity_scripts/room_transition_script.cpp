#include "room_transition_script.h"

#include "../entity_utils.h"

#include "../components/components.h"

#include "spdlog/spdlog.h"

namespace fck
{

RoomTransitionScript::RoomTransitionScript(Level *level)
    : m_level{level}, m_need_change_room{false}, m_used{false}
{
}

const std::string RoomTransitionScript::room() const
{
    return m_room;
}

void RoomTransitionScript::setRoom(const std::string &room)
{
    m_room = room;
}

const sf::Vector2f &RoomTransitionScript::point() const
{
    return m_point;
}

void RoomTransitionScript::setPoint(const sf::Vector2f &point)
{
    m_point = point;
}

void RoomTransitionScript::update(const Entity &entity, double delta_time)
{
    if (m_need_change_room)
    {
        spdlog::debug("Update move player");
        m_level->enableRoom(m_room, m_point);
        m_need_change_room = false;
    }
}

void RoomTransitionScript::onEntityEnabled(const Entity &entity)
{
    spdlog::debug("Enable transition");
    m_used = false;
}

void RoomTransitionScript::onEntityCollided(const Entity &entity, const Entity &other)
{
    if (!other.isValid() || !other.hasComponent<PlayerComponent>())
        return;

    if (!m_used)
    {
        spdlog::debug("Move player");
        m_need_change_room = true;
        m_used = true;
    }
}

} // namespace fck
