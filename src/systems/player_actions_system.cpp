#include "player_actions_system.h"

#include "../entity_utils.h"

#include <spdlog/spdlog.h>

namespace fck
{

PlayerActionsSystem::PlayerActionsSystem() : m_input_actions{nullptr}
{
}

void PlayerActionsSystem::setInputActions(InputActionsMap<keyboard_action::Action> *input_actions)
{
    if (m_input_actions)
        m_input_actions->action_activated.disconnect(m_action_activated_conenction);
    m_input_actions = input_actions;
    if (m_input_actions)
        m_action_activated_conenction = m_input_actions->action_activated.connect(
            this, &PlayerActionsSystem::onActionActivated);
}

void PlayerActionsSystem::update(double delta_time)
{
    if (!m_input_actions)
        return;

    for (Entity &entity : entities())
    {
        VelocityComponent &velocity_component = entity.component<VelocityComponent>();
        velocity_component.velocity = {0.0f, 0.0f};

        StateComponent &state_component = entity.component<StateComponent>();
        if (entity_state::NOT_AVALIBLE & state_component.state)
            continue;

        if (m_input_actions->isActive(keyboard_action::PLAYER_MOVE_LEFT))
            velocity_component.velocity.x -= velocity_component.max_velocity.x;

        if (m_input_actions->isActive(keyboard_action::PLAYER_MOVE_UP))
            velocity_component.velocity.y -= velocity_component.max_velocity.y;

        if (m_input_actions->isActive(keyboard_action::PLAYER_MOVE_RIGHT))
            velocity_component.velocity.x += velocity_component.max_velocity.x;

        if (m_input_actions->isActive(keyboard_action::PLAYER_MOVE_DOWN))
            velocity_component.velocity.y += velocity_component.max_velocity.y;

        if (velocity_component.velocity.x != 0 || velocity_component.velocity.y != 0)
            entity::set_state.emit(entity, entity_state::MOVE);
        else
            entity::set_state.emit(entity, entity_state::IDLE);

        if (velocity_component.velocity.x > 0)
            entity::set_direction.emit(entity, entity_state::RIGHT);
        else if (velocity_component.velocity.x < 0)
            entity::set_direction.emit(entity, entity_state::LEFT);
    }
}

void PlayerActionsSystem::onActionActivated(keyboard_action::Action action)
{
    for (Entity &entity : entities())
    {
        StateComponent &state_component = entity.component<StateComponent>();
        if (entity_state::NOT_AVALIBLE & state_component.state)
            continue;

        if (action == keyboard_action::CHANGE_TARGET)
        {
            PlayerComponent &player_component = entity.component<PlayerComponent>();
            player_component.need_change_target = true;
        }

        if (action == keyboard_action::PLAYER_SKILL_1)
        {
            SkillsComponent &skills_component = entity.component<SkillsComponent>();
            skills_component.next_skill = 0;
        }

        if (action == keyboard_action::PLAYER_SKILL_2)
        {
            SkillsComponent &skills_component = entity.component<SkillsComponent>();
            skills_component.next_skill = 1;
        }

        if (action == keyboard_action::PLAYER_SKILL_3)
        {
            SkillsComponent &skills_component = entity.component<SkillsComponent>();
            skills_component.next_skill = 2;
        }

        if (action == keyboard_action::PLAYER_SKILL_4)
        {
            SkillsComponent &skills_component = entity.component<SkillsComponent>();
            skills_component.next_skill = 3;
        }
    }
}

} // namespace fck
