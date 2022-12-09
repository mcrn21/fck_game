#include "room_transition.h"

#include "../entity_utils.h"

#include "../components/components.h"

#include "spdlog/spdlog.h"

namespace fck::entity_script
{

RoomTransition::RoomTransition(Level *level)
    : m_level{level}, m_need_change_room{false}, m_used{false}
{
}

void RoomTransition::setRoomCoord(const sf::Vector2i &room_coord)
{
    m_room_coord = room_coord;
}

void RoomTransition::setTargetPosition(const sf::Vector2f &target_position)
{
    m_target_position = target_position;
}

void RoomTransition::update(const Entity &entity, double delta_time)
{
    if (m_need_change_room)
    {
        spdlog::debug("Update move player");
        m_level->enableRoom(m_room_coord, m_target_position);
        m_need_change_room = false;
    }
}

void RoomTransition::onEntityEnabled(const Entity &entity)
{
    spdlog::debug("Enable transition");
    m_used = false;
}

void RoomTransition::onEntityCollided(const Entity &entity, const Entity &other)
{
    if (!other.isValid() || !other.hasComponent<component::Player>())
        return;

    if (!m_used)
    {
        spdlog::debug("Move player");
        m_need_change_room = true;
        m_used = true;
    }
}

} // namespace fck::entity_script
