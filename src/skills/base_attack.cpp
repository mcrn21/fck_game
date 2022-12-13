#include "base_attack.h"

#include "../entity_utils.h"

#include "../damages/damages.h"

#include "../components/components.h"

#include "../fck/utilities.h"

#include "spdlog/spdlog.h"

#include <random>

namespace fck::skill
{

BaseAttack::BaseAttack(
    const std::string &skill_name,
    double cooldown,
    const std::pair<float, float> &damage,
    const std::vector<std::string> &attack_animations,
    const std::pair<double, double> &jump_interval,
    const sf::Vector2f &target_jump_offset,
    const sf::Vector2f &idle_jump_offset)
    : SkillBase{skill_name, cooldown},
      m_damage{damage},
      m_attack_animations{attack_animations},
      m_jump_interval{jump_interval},
      m_target_jump_offset{target_jump_offset},
      m_idle_jump_offset{idle_jump_offset},
      m_target_attacked{false}
{
}

void BaseAttack::apply(const Entity &entity, const Entity &target)
{
    m_entity = entity;
    m_target = target;

    component::Transform &transform_component = m_entity.getComponent<component::Transform>();
    component::Velocity &velocity_component = m_entity.getComponent<component::Velocity>();
    component::State &state_component = m_entity.getComponent<component::State>();

    if (m_target.isValid())
    {
        component::Transform &target_transform_component
            = m_target.getComponent<component::Transform>();
        component::Scene &target_scene_component = m_target.getComponent<component::Scene>();

        m_jump_point = target_transform_component.transform.getPosition();
        m_jump_point.x += transform_component.transform.getPosition().x > m_jump_point.x
            ? (m_target_jump_offset.x + target_scene_component.global_bounds.width / 2)
            : -(m_target_jump_offset.x + target_scene_component.global_bounds.width / 2);

        entity::set_direction.emit(
            m_entity,
            m_jump_point.x > target_transform_component.transform.getPosition().x
                ? entity_state::LEFT
                : entity_state::RIGHT);

        m_target_attacked = false;

        m_attack_function = std::bind(&BaseAttack::targetAttack, this, std::placeholders::_1);
    }
    else
    {
        m_jump_point = transform_component.transform.getPosition();
        m_jump_point.x += state_component.direction == entity_state::LEFT ? -m_idle_jump_offset.x
                                                                          : m_idle_jump_offset.x;

        m_attack_function = std::bind(&BaseAttack::idleAttack, this, std::placeholders::_1);
    }

    float dist_to_point
        = vector2::distance(transform_component.transform.getPosition(), m_jump_point);
    float angle_to_point
        = vector2::angleTo(transform_component.transform.getPosition(), m_jump_point);

    float velocity = dist_to_point / (m_jump_interval.second - m_jump_interval.first);
    m_jump_velocity
        = sf::Vector2f(-velocity * std::cos(angle_to_point), -velocity * std::sin(angle_to_point));

    entity::set_state.emit(entity, entity_state::BASE_ATTACK);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int32_t> dist(0, m_attack_animations.size() - 1);

    entity::set_drawable_state.emit(m_entity, m_attack_animations[dist(mt)]);
}

void BaseAttack::update(double delta_time)
{
    m_attack_function(delta_time);

    if (isReady())
        entity::set_state.emit(m_entity, entity_state::IDLE);
}

void BaseAttack::idleAttack(double delta_time)
{
    component::Velocity &velocity_component = m_entity.getComponent<component::Velocity>();

    if (getEelapsed() >= m_jump_interval.first && getEelapsed() <= m_jump_interval.second)
    {
        velocity_component.velocity = m_jump_velocity;
        return;
    }

    velocity_component.velocity = {};
}

void BaseAttack::targetAttack(double delta_time)
{
    component::Velocity &velocity_component = m_entity.getComponent<component::Velocity>();
    if (getEelapsed() >= m_jump_interval.first && getEelapsed() <= m_jump_interval.second)
    {
        velocity_component.velocity = m_jump_velocity;
        return;
    }

    velocity_component.velocity = {};

    if (!m_target.isValid())
        return;

    if (!m_target_attacked)
    {
        component::State &target_state_component = m_target.getComponent<component::State>();
        if (target_state_component.state != entity_state::DAMAGED
            && target_state_component.state != entity_state::DEATH)
        {
            component::Transform &target_transform_component
                = m_target.getComponent<component::Transform>();
            sf::Vector2f rebounce_velocity
                = {m_jump_point.x > target_transform_component.transform.getPosition().x ? -100.0f
                                                                                         : 100.0f,
                   0.0f};

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<float> dist(m_damage.first, m_damage.second);

            component::Damage &target_damage_component = m_target.getComponent<component::Damage>();
            if (!target_damage_component.damage)
            {
                target_damage_component.damage.reset(
                    new damage::BaseAttack(dist(mt), rebounce_velocity, m_target, 0.2, m_entity));
            }
        }
        m_target_attacked = true;
    }
}

} // namespace fck::skill
