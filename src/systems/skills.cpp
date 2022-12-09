#include "skills.h"

namespace fck::system
{

Skills::Skills()
{
}

void Skills::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        component::Skills &skills_component = entity.component<component::Skills>();

        if (skills_component.next_skill != -1)
        {
            component::State &state_component = entity.component<component::State>();

            // If entity already attacked, damaged or die ignore skill
            if (skills_component.next_skill < skills_component.skills.size()
                && !(entity_state::NOT_AVALIBLE & state_component.state))
            {
                Entity target;
                if (entity.hasComponent<component::Target>())
                {
                    component::Target &target_component
                        = entity.component<component::Target>();
                    if (target_component.target.isValid())
                    {
                        component::State &target_state_component
                            = target_component.target.component<component::State>();
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
                component::State &state_component = entity.component<component::State>();
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

} // namespace fck::system
