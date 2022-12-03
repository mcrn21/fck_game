#include "skills_system.h"

namespace fck
{

SkillsSystem::SkillsSystem()
{
}

void SkillsSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        SkillsComponent &skills_component = entity.component<SkillsComponent>();

        if (skills_component.next_skill != -1)
        {
            StateComponent &state_component = entity.component<StateComponent>();

            // If entity already attacked, damaged or die ignore skill
            if (skills_component.next_skill < skills_component.skills.size()
                && !(entity_state::NOT_AVALIBLE & state_component.state))
            {
                Entity target;
                if (entity.hasComponent<TargetComponent>())
                {
                    TargetComponent &target_component = entity.component<TargetComponent>();
                    if (target_component.target.isValid())
                    {
                        StateComponent &target_state_component
                            = target_component.target.component<StateComponent>();
                        if (target_state_component.state != entity_state::DEATH)
                            target = target_component.target;
                    }
                }

                if (skills_component.skills[skills_component.next_skill]->isReady())
                    skills_component.skills[skills_component.next_skill]->_apply(entity, target);
            }

            skills_component.next_skill = -1;
        }

        for (auto &skill : skills_component.skills)
        {
            if (!skill->isReady())
            {
                // If entity die finish skill
                StateComponent &state_component = entity.component<StateComponent>();
                if (state_component.state == entity_state::DEATH)
                {
                    skill->finish();
                    continue;
                }

                skill->_update(delta_time);
            }
        }
    }
}

} // namespace fck
