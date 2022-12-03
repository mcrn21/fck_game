#include "enemy_script_base.h"
#include "../components/components.h"

namespace fck
{

EnemyScriptBase::EnemyScriptBase(double attack_interval) : m_attack_interval{attack_interval}
{
}

void EnemyScriptBase::update(const Entity &entity, double delta_time)
{
    TargetFollowComponent &target_follow_component = entity.component<TargetFollowComponent>();
    TargetComponent &target_component = entity.component<TargetComponent>();

    if (target_follow_component.state == TargetFollowComponent::RICHED)
    {
        m_attack_interval -= delta_time;

        if (m_attack_interval < 0)
        {
            SkillsComponent &skills_component = entity.component<SkillsComponent>();
            skills_component.next_skill = 0;
            m_attack_interval = 0.5;
        }
    }
}

} // namespace fck
