#include "actions_system.h"

#include <spdlog/spdlog.h>

namespace fck
{

ActionsSystem::ActionsSystem()
{
}

void ActionsSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        ActionsComponent &actions_component = entity.component<ActionsComponent>();
        if (actions_component.next_action != -1)
        {
            StateComponent &state_component = entity.component<StateComponent>();

            if (actions_component.next_action < actions_component.actions.size()
                && state_component.state != entity_state::ACTION
                && state_component.state != entity_state::UNDER_ACTION)
            {
                std::vector<Entity> targets;
                if (entity.hasComponent<TargetComponent>())
                {
                    TargetComponent &target = entity.component<TargetComponent>();
                    if (target.target.isValid())
                        targets.push_back(target.target);
                }

                if (actions_component.actions[actions_component.next_action]->isReady())
                {
                    ActionResultBase *result
                        = actions_component.actions[actions_component.next_action]->apply(
                            entity, targets);
                    m_results.push_back(std::unique_ptr<ActionResultBase>(result));
                    m_cooldowning_actions.push_front(
                        actions_component.actions[actions_component.next_action].get());
                }
            }

            actions_component.next_action = -1;
        }
    }

    updateResults(delta_time);
    updateActions(delta_time);
}

void ActionsSystem::onEntityRemoved(Entity &entity)
{
    ActionsComponent &actions_component = entity.component<ActionsComponent>();
    for (auto &action : actions_component.actions)
    {
        m_cooldowning_actions.erase(
            std::remove(m_cooldowning_actions.begin(), m_cooldowning_actions.end(), action.get()),
            m_cooldowning_actions.end());
    }
}

void ActionsSystem::updateResults(double delta_time)
{
    for (auto it = m_results.begin(); it != m_results.end();)
    {
        if ((*it)->update(delta_time))
            it = m_results.erase(it);
        else
            ++it;
    }
}

void ActionsSystem::updateActions(double delta_time)
{
    for (auto it = m_cooldowning_actions.begin(); it != m_cooldowning_actions.end();)
    {
        (*it)->update(delta_time);
        if ((*it)->isReady())
            it = m_cooldowning_actions.erase(it);
        else
            ++it;
    }
}

} // namespace fck
