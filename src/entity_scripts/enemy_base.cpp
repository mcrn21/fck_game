#include "enemy_base.h"

#include "../components/components.h"

namespace fck::entity_script
{

EnemyBase::EnemyBase(double attack_interval) : m_attack_interval{attack_interval}
{
}

void EnemyBase::update(const Entity &entity, double delta_time)
{
    component::TargetFollow &target_follow_component
        = entity.getComponent<component::TargetFollow>();
    component::Target &target_component = entity.getComponent<component::Target>();
    component::LookAround &look_around_component = entity.getComponent<component::LookAround>();
    component::State &state_component = entity.getComponent<component::State>();

    if (state_component.state == entity_state::DEATH)
    {
        target_component.target = Entity{};
        look_around_component.enable = false;
        target_follow_component.follow = false;
        return;
    }

    if (!target_component.target.isValid())
    {
        for (const Entity &found_entity : look_around_component.found_entities)
        {
            spdlog::debug("Find player");
            if (found_entity.hasComponent<component::Player>())
            {
                target_component.target = found_entity;
                target_follow_component.follow = true;
                look_around_component.enable = false;
                break;
            }
        }
    }

    if (target_component.target.isValid())
    {
        component::Scene &target_scene_component
            = target_component.target.getComponent<component::Scene>();

        if (!look_around_component.global_bounds
                 .findIntersection(target_scene_component.global_bounds)
                 .has_value())
        {
            target_component.target = Entity{};
            look_around_component.enable = true;
            target_follow_component.follow = false;
            return;
        }

        component::Transform &target_transform_component
            = target_component.target.getComponent<component::Transform>();
        target_follow_component.target = target_transform_component.transform.getPosition();

        if (target_follow_component.state == component::TargetFollow::RICHED)
        {
            m_attack_interval -= delta_time;
            if (m_attack_interval < 0)
            {
                component::Skills &skills_component = entity.getComponent<component::Skills>();
                skills_component.next_skill = 0;
                m_attack_interval = 0.5;
            }
        }
    }
}

} // namespace fck::entity_script
