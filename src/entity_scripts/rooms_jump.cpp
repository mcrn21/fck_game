#include "rooms_jump.h"

#include "../entity_utils.h"

#include "../components/components.h"

#include "spdlog/spdlog.h"

namespace fck::entity_script
{

RoomsJump::RoomsJump(Level *level) : m_level{level}, m_need_change_room{false}, m_used{false}
{
}

void RoomsJump::setSide(room_side::Side side)
{
    m_side = side;
}

void RoomsJump::setRoomCoord(const sf::Vector2i &room_coord)
{
    m_room_coord = room_coord;
}

void RoomsJump::update(const Entity &entity, double delta_time)
{
    if (m_need_change_room)
    {
        sf::Vector2f target_position;

        if (m_side == room_side::LEFT)
        {
            target_position = m_level->getRoomsMap()
                                  .getData(m_room_coord)
                                  ->getEntryPoints()
                                  .at(room_side::RIGHT);
        }
        else if (m_side == room_side::RIGHT)
        {
            target_position = m_level->getRoomsMap()
                                  .getData(m_room_coord)
                                  ->getEntryPoints()
                                  .at(room_side::LEFT);
        }
        else if (m_side == room_side::TOP)
        {
            target_position = m_level->getRoomsMap()
                                  .getData(m_room_coord)
                                  ->getEntryPoints()
                                  .at(room_side::BOTTOM);
        }
        else if (m_side == room_side::BOTTOM)
        {
            target_position
                = m_level->getRoomsMap().getData(m_room_coord)->getEntryPoints().at(room_side::TOP);
        }

        m_level->enableRoom(m_room_coord, target_position);
        m_need_change_room = false;
    }
}

void RoomsJump::onEntityEnabled(const Entity &entity)
{
    m_used = false;
}

void RoomsJump::onEntityCollided(const Entity &entity, const Entity &other)
{
    if (!other.isValid() || !other.has<component::Player>())
        return;

    if (!m_used)
    {
        m_need_change_room = true;
        m_used = true;
    }
}

} // namespace fck::entity_script
