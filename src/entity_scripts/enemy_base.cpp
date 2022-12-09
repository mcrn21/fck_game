#include "enemy_base.h"

#include "../components/components.h"

namespace fck::entity_script
{

EnemyBase::EnemyBase(double attack_interval) : m_attack_interval{attack_interval}
{
}

void EnemyBase::update(const Entity &entity, double delta_time)
{
    component::TargetFollow &target_follow_component = entity.component<component::TargetFollow>();
    component::Target &target_component = entity.component<component::Target>();
    component::LookAround &look_around_component = entity.component<component::LookAround>();

    //    if (!target_component.target.isValid())
    //    {
    //        for (const Entity &entity : look_around_component.look_at_entities)
    //        {
    //            if (entity.hasComponent<PlayerComponent>())
    //            {
    //                target_component.target = entity;
    //                target_follow_component.follow = true;
    //            }
    //        }
    //    }

    if (target_follow_component.state == component::TargetFollow::RICHED)
    {
        m_attack_interval -= delta_time;

        if (m_attack_interval < 0)
        {
            component::Skills &skills_component = entity.component<component::Skills>();
            skills_component.next_skill = 0;
            m_attack_interval = 0.5;
        }
    }
}

} // namespace fck::entity_script
