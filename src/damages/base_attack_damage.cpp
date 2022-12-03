#include "base_attack_damage.h"

#include "../entity_utils.h"

#include "../components/components.h"

#include <spdlog/spdlog.h>

namespace fck
{

BaseAttackDamage::BaseAttackDamage(
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

void BaseAttackDamage::update(double delta_time)
{
    if (!entity().isValid())
        return;

    if (m_first_update)
    {
        StatsComponent &stats_component = entity().component<StatsComponent>();
        stats_component.damage = m_damage;

        if (source().isValid())
        {
            if (entity().hasComponent<TargetComponent>())
            {
                TargetComponent &target_component = entity().component<TargetComponent>();
                target_component.target = source();
            }
        }

        StateComponent &state_component = entity().component<StateComponent>();
        if (!(entity_state::ATTACK & state_component.state))
            EntityUtils::setEntityState(entity(), entity_state::DAMAGED);

        m_first_update = false;
    }

    VelocityComponent &velocity_component = entity().component<VelocityComponent>();

    if (elapsed() >= m_rebounce_interval.first && elapsed() <= m_rebounce_interval.second)
    {
        velocity_component.velocity = m_rebound_velocity;
        return;
    }

    velocity_component.velocity = {};

    if (isReady())
    {
        StateComponent &state_component = entity().component<StateComponent>();
        if (!(entity_state::ATTACK & state_component.state))
            EntityUtils::setEntityState(entity(), entity_state::IDLE);
    }
}

} // namespace fck
