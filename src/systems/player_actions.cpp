#include "player_actions.h"

#include "../entity_utils.h"

#include <spdlog/spdlog.h>

namespace fck::system
{

PlayerActions::PlayerActions() : m_input_actions{nullptr}
{
}

void PlayerActions::setInputActions(InputActionsMap<keyboard_action::Action> *input_actions)
{
    if (m_input_actions)
        m_input_actions->action_activated.disconnect(m_action_activated_conenction);
    m_input_actions = input_actions;
    if (m_input_actions)
        m_action_activated_conenction
            = m_input_actions->action_activated.connect(this, &PlayerActions::onActionActivated);
}

void PlayerActions::update(double delta_time)
{
    if (!m_input_actions)
        return;

    for (Entity &entity : getEntities())
    {
        component::Velocity &velocity_component
            = entity.getComponent<component::Velocity>();
        velocity_component.velocity = {0.0f, 0.0f};

        component::State &state_component = entity.getComponent<component::State>();
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

void PlayerActions::onActionActivated(keyboard_action::Action action)
{
    for (Entity &entity : getEntities())
    {
        if (action == keyboard_action::CHANGE_TARGET)
        {
            component::Player &player_component = entity.getComponent<component::Player>();
            player_component.need_change_target = true;
        }

        if (action == keyboard_action::PLAYER_SKILL_1)
        {
            component::Skills &skills_component = entity.getComponent<component::Skills>();
            skills_component.next_skill = 0;
        }

        if (action == keyboard_action::PLAYER_SKILL_2)
        {
            component::Skills &skills_component = entity.getComponent<component::Skills>();
            skills_component.next_skill = 1;
        }

        if (action == keyboard_action::PLAYER_SKILL_3)
        {
            component::Skills &skills_component = entity.getComponent<component::Skills>();
            skills_component.next_skill = 2;
        }

        if (action == keyboard_action::PLAYER_SKILL_4)
        {
            component::Skills &skills_component = entity.getComponent<component::Skills>();
            skills_component.next_skill = 3;
        }
    }
}

} // namespace fck::system
