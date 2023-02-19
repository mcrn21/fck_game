#include "player.h"
#include "../entity_funcs.h"
#include "../knowledge_base/knowledge_base.h"

#include <spdlog/spdlog.h>

namespace fck::system
{

Player::Player() : m_move_direction{0}
{
}

void Player::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::Velocity &velocity_component = entity.get<component::Velocity>();
        velocity_component.velocity = {0.0f, 0.0f};

        component::State &state_component = entity.get<component::State>();
        if (entity_state::NOT_AVALIBLE & state_component.state)
            continue;

        if (m_move_direction & LEFT)
            velocity_component.velocity.x -= velocity_component.max_velocity.x;

        if (m_move_direction & UP)
            velocity_component.velocity.y -= velocity_component.max_velocity.y;

        if (m_move_direction & RIGHT)
            velocity_component.velocity.x += velocity_component.max_velocity.x;

        if (m_move_direction & DOWN)
            velocity_component.velocity.y += velocity_component.max_velocity.y;

        if (velocity_component.velocity.x > 0)
            entity_funcs::setDirection(entity, entity_state::RIGHT);
        else if (velocity_component.velocity.x < 0)
            entity_funcs::setDirection(entity, entity_state::LEFT);

        if (m_move_direction != 0 && state_component.state == entity_state::IDLE)
        {
            entity_funcs::setState(entity, entity_state::MOVE);
            entity_funcs::setDrawableState(entity, entity_state::stateToString(entity_state::MOVE));
            entity_funcs::playSound(entity, entity_state::stateToString(entity_state::MOVE));
        }
        else if (m_move_direction == 0 && state_component.state == entity_state::MOVE)
        {
            entity_funcs::setState(entity, entity_state::IDLE);
            entity_funcs::setDrawableState(entity, entity_state::stateToString(entity_state::IDLE));
            entity_funcs::stopSound(entity, entity_state::stateToString(entity_state::MOVE));
        }
    }
}

void Player::onActionActivated(keyboard_action::Action action)
{
    if (action == keyboard_action::PLAYER_MOVE_LEFT)
    {
        m_move_direction |= LEFT;
    }
    else if (action == keyboard_action::PLAYER_MOVE_UP)
    {
        m_move_direction |= UP;
    }
    else if (action == keyboard_action::PLAYER_MOVE_RIGHT)
    {
        m_move_direction |= RIGHT;
    }
    else if (action == keyboard_action::PLAYER_MOVE_DOWN)
    {
        m_move_direction |= DOWN;
    }

    for (Entity &entity : getEntities())
    {
        auto &player_component = entity.get<component::Player>();
        auto &skills_component = entity.get<component::Skills>();

        if (action == keyboard_action::CHANGE_TARGET)
            player_component.need_change_target = true;

        if (action == keyboard_action::PLAYER_SKILL_1)
            skills_component.next_skill = 0;

        if (action == keyboard_action::PLAYER_SKILL_2)
            skills_component.next_skill = 1;

        if (action == keyboard_action::PLAYER_SKILL_3)
            skills_component.next_skill = 2;

        if (action == keyboard_action::PLAYER_SKILL_4)
            skills_component.next_skill = 3;
    }
}

void Player::onActionDiactivated(keyboard_action::Action action)
{
    if (action == keyboard_action::PLAYER_MOVE_LEFT)
    {
        m_move_direction &= ~(LEFT);
    }
    else if (action == keyboard_action::PLAYER_MOVE_UP)
    {
        m_move_direction &= ~(UP);
    }
    else if (action == keyboard_action::PLAYER_MOVE_RIGHT)
    {
        m_move_direction &= ~(RIGHT);
    }
    else if (action == keyboard_action::PLAYER_MOVE_DOWN)
    {
        m_move_direction &= ~(DOWN);
    }
}

void Player::onEntityTargetChanged(
    const Entity &entity, const Entity &target, const Entity &old_target)
{
    if (!entity.has<component::Player>())
        return;

    auto &player_component = entity.get<component::Player>();

    if (old_target.isValid())
        entity_funcs::setMarker(old_target, Entity{});

    if (target.isValid())
        entity_funcs::setMarker(target, EntityFactory::createEntity("target", getWorld()));
}

} // namespace fck::system
