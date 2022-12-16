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

void RoomTransition::setSide(Room::Side side)
{
    m_side = side;
}

void RoomTransition::setRoomCoord(const sf::Vector2i &room_coord)
{
    m_room_coord = room_coord;
}

void RoomTransition::update(const Entity &entity, double delta_time)
{
    if (m_need_change_room)
    {
        sf::Vector2f target_position;

        if (m_side == Room::LEFT)
        {
            target_position
                = m_level->getRoomsMap().getData(m_room_coord)->getEntryPoints().at(Room::RIGHT);
        }
        else if (m_side == Room::RIGHT)
        {
            target_position
                = m_level->getRoomsMap().getData(m_room_coord)->getEntryPoints().at(Room::LEFT);
        }
        else if (m_side == Room::TOP)
        {
            target_position
                = m_level->getRoomsMap().getData(m_room_coord)->getEntryPoints().at(Room::BOTTOM);
        }
        else if (m_side == Room::BOTTOM)
        {
            target_position
                = m_level->getRoomsMap().getData(m_room_coord)->getEntryPoints().at(Room::TOP);
        }

        m_level->enableRoom(m_room_coord, target_position);
        m_need_change_room = false;
    }
}

void RoomTransition::onEntityEnabled(const Entity &entity)
{
    m_used = false;
}

void RoomTransition::onEntityCollided(const Entity &entity, const Entity &other)
{
    if (!other.isValid() || !other.hasComponent<component::Player>())
        return;

    if (!m_used)
    {
        m_need_change_room = true;
        m_used = true;
    }
}

} // namespace fck::entity_script
