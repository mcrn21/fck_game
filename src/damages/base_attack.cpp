#include "base_attack.h"

#include "../entity_utils.h"

#include "../components/components.h"

#include <spdlog/spdlog.h>

namespace fck::damage
{

BaseAttack::BaseAttack(
    float damage,
    const sf::Vector2f rebound_velocity,
    const Entity &entity,
    double interval,
    const Entity &source)
    : DamageBase{entity, interval, source},
      m_damage{damage},
      m_rebound_velocity{rebound_velocity},
      m_first_update{true},
      m_damaged{false}
{
    m_rebounce_interval = {0.0f, 0.05f};
}

void BaseAttack::update(double delta_time)
{
    if (!entity().isValid())
        return;

    if (m_first_update)
    {
        component::Stats &stats_component = entity().component<component::Stats>();
        stats_component.damage = m_damage;

        //        if (source().isValid())
        //        {
        //            if (entity().hasComponent<TargetComponent>())
        //            {
        //                TargetComponent &target_component = entity().component<TargetComponent>();
        //                target_component.target = source();
        //            }
        //        }

        component::State &state_component = entity().component<component::State>();
        if (!(entity_state::ATTACK & state_component.state))
            entity::set_state.emit(entity(), entity_state::DAMAGED);

        m_first_update = false;
    }

    component::Velocity &velocity_component = entity().component<component::Velocity>();

    if (elapsed() >= m_rebounce_interval.first && elapsed() <= m_rebounce_interval.second)
    {
        velocity_component.velocity = m_rebound_velocity;
        return;
    }

    velocity_component.velocity = {};

    component::State &state_component = entity().component<component::State>();
    if (!(entity_state::ATTACK & state_component.state))
        entity::set_state.emit(entity(), entity_state::IDLE);
}

} // namespace fck::damage
