#include "base_attack_skill.h"

#include "../entity_utils.h"

#include "../damages/damages.h"

#include "../components/components.h"

#include "../fck/utilities.h"

#include "spdlog/spdlog.h"

#include <random>

namespace fck
{

BaseAttackSkill::BaseAttackSkill(
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

void BaseAttackSkill::apply(const Entity &entity, const Entity &target)
{
    m_entity = entity;
    m_target = target;

    TransformComponent &transform_component = m_entity.component<TransformComponent>();
    VelocityComponent &velocity_component = m_entity.component<VelocityComponent>();
    StateComponent &state_component = m_entity.component<StateComponent>();
    DrawableAnimationComponent &drawable_animation_component
        = m_entity.component<DrawableAnimationComponent>();

    if (m_target.isValid())
    {
        TransformComponent &target_transform_component = m_target.component<TransformComponent>();
        SceneComponent &target_scene_component = m_target.component<SceneComponent>();

        m_jump_point = target_transform_component.transform.getPosition();
        m_jump_point.x += transform_component.transform.getPosition().x > m_jump_point.x
            ? (m_target_jump_offset.x + target_scene_component.global_bounds.width / 2)
            : -(m_target_jump_offset.x + target_scene_component.global_bounds.width / 2);

        EntityUtils::setEntityDirection(
            m_entity,
            m_jump_point.x > target_transform_component.transform.getPosition().x
                ? entity_state::LEFT
                : entity_state::RIGHT);

        m_target_attacked = false;

        m_attack_function = std::bind(&BaseAttackSkill::targetAttack, this, std::placeholders::_1);
    }
    else
    {
        m_jump_point = transform_component.transform.getPosition();
        m_jump_point.x += state_component.direction == entity_state::LEFT ? -m_idle_jump_offset.x
                                                                          : m_idle_jump_offset.x;

        m_attack_function = std::bind(&BaseAttackSkill::idleAttack, this, std::placeholders::_1);
    }

    float dist_to_point
        = vector2::distance(transform_component.transform.getPosition(), m_jump_point);
    float angle_to_point
        = vector2::angleTo(transform_component.transform.getPosition(), m_jump_point);

    float velocity = dist_to_point / (m_jump_interval.second - m_jump_interval.first);
    m_jump_velocity
        = sf::Vector2f(-velocity * std::cos(angle_to_point), -velocity * std::sin(angle_to_point));

    EntityUtils::setEntityState(entity, entity_state::BASE_ATTACK);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int32_t> dist(0, m_attack_animations.size() - 1);

    drawable_animation_component.drawable_animation->setCurrentState(m_attack_animations[dist(mt)]);
    drawable_animation_component.drawable_animation->start();
}

void BaseAttackSkill::update(double delta_time)
{
    m_attack_function(delta_time);

    if (isReady())
        EntityUtils::setEntityState(m_entity, entity_state::IDLE);
}

void BaseAttackSkill::idleAttack(double delta_time)
{
    VelocityComponent &velocity_component = m_entity.component<VelocityComponent>();

    if (elapsed() >= m_jump_interval.first && elapsed() <= m_jump_interval.second)
    {
        velocity_component.velocity = m_jump_velocity;
        return;
    }

    velocity_component.velocity = {};
}

void BaseAttackSkill::targetAttack(double delta_time)
{
    VelocityComponent &velocity_component = m_entity.component<VelocityComponent>();
    if (elapsed() >= m_jump_interval.first && elapsed() <= m_jump_interval.second)
    {
        velocity_component.velocity = m_jump_velocity;
        return;
    }

    velocity_component.velocity = {};

    if (!m_target.isValid())
        return;

    if (!m_target_attacked)
    {
        StateComponent &target_state_component = m_target.component<StateComponent>();
        if (target_state_component.state != entity_state::DAMAGED
            && target_state_component.state != entity_state::DEATH)
        {
            TransformComponent &target_transform_component
                = m_target.component<TransformComponent>();
            sf::Vector2f rebounce_velocity
                = {m_jump_point.x > target_transform_component.transform.getPosition().x ? -100.0f
                                                                                         : 100.0f,
                   0.0f};

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<float> dist(m_damage.first, m_damage.second);

            DamageComponent &target_damage_component = m_target.component<DamageComponent>();
            if (!target_damage_component.damage)
            {
                target_damage_component.damage.reset(
                    new BaseAttackDamage(dist(mt), rebounce_velocity, m_target, 0.2, m_entity));
            }
        }
        m_target_attacked = true;
    }
}

} // namespace fck
