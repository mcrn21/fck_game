#include "skills.h"

#include "../entity_funcs.h"

namespace fck::system
{

Skills::Skills()
{
}

void Skills::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::Skills &skills_component = entity.get<component::Skills>();

        if (skills_component.next_skill != -1)
        {
            component::State &state_component = entity.get<component::State>();

            // If entity already attacked, damaged or die ignore skill
            if (skills_component.next_skill < skills_component.skills.size()
                && !(entity_state::NOT_AVALIBLE & state_component.state))
            {

                if (skills_component.skills[skills_component.next_skill]->isReady())
                {
                    skills_component.skills[skills_component.next_skill]->start();
                    entity_funcs::skill_applied(
                        entity, skills_component.skills[skills_component.next_skill].get());
                }
            }

            skills_component.next_skill = -1;
        }

        for (auto &skill : skills_component.skills)
        {
            if (!skill->isReady())
            {
                // If entity die finish skill
                component::State &state_component = entity.get<component::State>();
                if (state_component.state == entity_state::DEATH)
                {
                    skill->finish();
                    entity_funcs::skill_finished(entity, skill.get());
                    continue;
                }

                skill->update(delta_time);

                if (skill->isReady())
                    entity_funcs::skill_finished(entity, skill.get());
            }
        }
    }
}

} // namespace fck::system
