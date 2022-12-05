#include "room_transition_script.h"

#include "../entity_utils.h"

#include "../components/components.h"

#include "spdlog/spdlog.h"

namespace fck
{

RoomTransitionScript::RoomTransitionScript(Level *level) : m_level{level}, m_need_change_room{false}
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
        m_level->enableRoom(m_room);
        entity::set_position.emit(m_transition_entity, m_point);

        PlayerComponent &player_component = m_transition_entity.component<PlayerComponent>();
        player_component.view_hard_set_position = true;

        m_need_change_room = false;
        m_transition_entity = Entity{};
    }
}

void RoomTransitionScript::onEntityCollided(const Entity &entity, const Entity &other)
{
    if (!other.isValid() || !other.hasComponent<PlayerComponent>())
        return;

    m_need_change_room = true;
    m_transition_entity = other;
}

} // namespace fck
