#include "test_action.h"

#include "../entity_utils.h"

#include "../components/components.h"

#include "../fck/sprite_animation.h"
#include "../fck/utilities.h"

#include <spdlog/spdlog.h>

#include <random>

namespace fck
{

TestActionResult::TestActionResult(const Entity &entity, const std::vector<Entity> &targes)
    : m_entity{entity},
      m_targets{targes},
      m_first_update{true},
      m_damaged{false},
      m_interval{0},
      m_max_interval{0.400}
{
}

bool TestActionResult::update(double delta_time)
{
    if (!m_entity.isValid())
        return true;

    TransformComponent &transform = m_entity.component<TransformComponent>();
    VelocityComponent &velocity = m_entity.component<VelocityComponent>();
    StateComponent &state = m_entity.component<StateComponent>();

    DrawableAnimationComponent &drawable_animation
        = m_entity.component<DrawableAnimationComponent>();
    SpriteAnimation *sprite_animation
        = static_cast<SpriteAnimation *>(drawable_animation.drawable_animation.get());

    bool target_valid = false;
    if (!m_targets.empty() && m_targets.front().isValid())
    {
        Entity &target = m_targets.front();
        StateComponent &target_state_component = target.component<StateComponent>();
        if (target_state_component.state != entity_state::DEATH)
            target_valid = true;
    }

    if (m_first_update)
    {
        if (target_valid)
        {
            Entity &target = m_targets.front();
            TransformComponent &target_transform = target.component<TransformComponent>();
            VelocityComponent &target_velocity = target.component<VelocityComponent>();

            m_point = target_transform.transform.getPosition();
            m_point.x
                += transform.transform.getPosition().x < target_transform.transform.getPosition().x
                ? -30.0f
                : 30.0f;

            EntityUtils::setEntityState(target, entity_state::UNDER_ACTION);

            DrawableAnimationComponent &target_drawable_animation
                = target.component<DrawableAnimationComponent>();
            SpriteAnimation *target_sprite_animation = static_cast<SpriteAnimation *>(
                target_drawable_animation.drawable_animation.get());

            target_sprite_animation->setCurrentState("under_attack");
            sprite_animation->start();
        }
        else
        {
            m_point = transform.transform.getPosition();
            m_point.x += state.direction == entity_state::LEFT ? -20.0f : 20.0f;
        }

        EntityUtils::setEntityState(m_entity, entity_state::ACTION);

        std::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int_distribution<int32_t> dist(1, 2);
        std::string animation_name = "attack_" + std::to_string(dist(mt));

        sprite_animation->setCurrentState(animation_name);
        sprite_animation->start();

        m_first_update = false;
    }

    m_interval += delta_time;

    if (m_interval < 0.200)
    {
        float dist_to_target = vector2::distance(transform.transform.getPosition(), m_point);
        float angle = vector2::angleTo(transform.transform.getPosition(), m_point);
        float dist = vector2::distance(transform.transform.getPosition(), m_point);
        velocity.velocity
            = sf::Vector2f(-dist * 30 * std::cos(angle), -dist * 30 * std::sin(angle));
        return false;
    }

    if (m_interval < 0.240)
    {
        if (target_valid)
        {
            Entity &target = m_targets.front();
            VelocityComponent &target_velocity = target.component<VelocityComponent>();
            TransformComponent &target_transform = target.component<TransformComponent>();
            StatsComponent &target_stats = target.component<StatsComponent>();

            target_velocity.velocity.x
                = transform.transform.getPosition().x < target_transform.transform.getPosition().x
                ? 200.0f
                : -200.0f;

            if (!m_damaged)
            {
                target_stats.damage = 20;
                m_damaged = true;
            }
            return false;
        }
    }

    if (target_valid)
    {
        Entity &target = m_targets.front();
        VelocityComponent &target_velocity = target.component<VelocityComponent>();
        target_velocity.velocity = {};
    }

    if (m_interval < m_max_interval)
        return false;

    velocity.velocity = {0.0f, 0.0f};

    EntityUtils::setEntityState(m_entity, entity_state::IDLE);

    if (target_valid)
    {
        Entity &target = m_targets.front();
        EntityUtils::setEntityState(target, entity_state::IDLE);
    }

    spdlog::debug("Action complited");
    return true;
}

TestAction::TestAction() : ActionBase{0.400}
{
}

ActionResultBase *TestAction::createResult(const Entity &entity, const std::vector<Entity> &targets)
{
    return new TestActionResult{entity, targets};
}

} // namespace fck
