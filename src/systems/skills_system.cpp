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

            if (skills_component.next_skill < skills_component.skills.size())
            {
                Entity target;
                if (entity.hasComponent<TargetComponent>())
                {
                    TargetComponent &target_component = entity.component<TargetComponent>();
                    if (target_component.target.isValid())
                        target = target_component.target;
                }

                if (skills_component.skills[skills_component.next_skill]->isReady())
                    skills_component.skills[skills_component.next_skill]->_apply(entity, target);
            }

            skills_component.next_skill = -1;
        }

        for (auto &skill : skills_component.skills)
        {
            if (!skill->isReady())
                skill->_update(delta_time);
        }
    }
}

} // namespace fck
