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

void RoomTransitionScript::setRoomCoord(const sf::Vector2i &room_coord)
{
    m_room_coord = room_coord;
}

void RoomTransitionScript::setTargetPosition(const sf::Vector2f &target_position)
{
    m_target_position = target_position;
}

void RoomTransitionScript::update(const Entity &entity, double delta_time)
{
    if (m_need_change_room)
    {
        spdlog::debug("Update move player");
        m_level->enableRoom(m_room_coord, m_target_position);
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
