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
    if (!getEntity().isValid())
        return;

    if (m_first_update)
    {
        component::Stats &stats_component = getEntity().getComponent<component::Stats>();
        stats_component.damage = m_damage;

        //        if (source().isValid())
        //        {
        //            if (entity().hasComponent<TargetComponent>())
        //            {
        //                TargetComponent &target_component = entity().component<TargetComponent>();
        //                target_component.target = source();
        //            }
        //        }

        component::State &state_component = getEntity().getComponent<component::State>();
        if (!(entity_state::ATTACK & state_component.state))
        {
            entity::set_state.emit(getEntity(), entity_state::DAMAGED);
            entity::set_drawable_state.emit(
                getEntity(), entity_state::stateToString(entity_state::DAMAGED));
            entity::play_sound.emit(getEntity(), "damaged");
        }

        m_first_update = false;
    }

    component::Velocity &velocity_component = getEntity().getComponent<component::Velocity>();

    if (getElapsed() >= m_rebounce_interval.first && getElapsed() <= m_rebounce_interval.second)
    {
        velocity_component.velocity = m_rebound_velocity;
        return;
    }

    velocity_component.velocity = {};

    component::State &state_component = getEntity().getComponent<component::State>();
    if (state_component.state == entity_state::DAMAGED)
    {
        entity::set_state.emit(getEntity(), entity_state::IDLE);
        entity::set_drawable_state.emit(
            getEntity(), entity_state::stateToString(entity_state::IDLE));
    }
}

} // namespace fck::damage
