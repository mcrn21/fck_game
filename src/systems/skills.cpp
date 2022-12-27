#include "skills.h"

#include "../entity_utils.h"

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
                Entity target;
                if (entity.has<component::Target>())
                {
                    component::Target &target_component = entity.get<component::Target>();
                    if (target_component.target.isValid())
                    {
                        component::State &target_state_component
                            = target_component.target.get<component::State>();
                        if (target_state_component.state != entity_state::DEATH)
                            target = target_component.target;
                    }
                }

                if (skills_component.skills[skills_component.next_skill]->isReady())
                {
                    skills_component.skills[skills_component.next_skill]->_apply(entity, target);
                    entity::skill_applied.emit(
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
                    entity::skill_finished.emit(entity, skill.get());
                    continue;
                }

                skill->_update(delta_time);

                if (skill->isReady())
                    entity::skill_finished.emit(entity, skill.get());
            }
        }
    }
}

} // namespace fck::system
