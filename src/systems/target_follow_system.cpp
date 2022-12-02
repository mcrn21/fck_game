#include "target_follow_system.h"

#include "../entity_utils.h"

#include "../fck/utilities.h"

namespace fck
{

TargetFollowSystem::TargetFollowSystem(PathFinder *path_finder) : m_path_finder{path_finder}
{
}

void TargetFollowSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        TargetComponent &target_component = entity.component<TargetComponent>();
        VelocityComponent &velocity_component = entity.component<VelocityComponent>();
        TransformComponent &transform_component = entity.component<TransformComponent>();
        StateComponent &state_component = entity.component<StateComponent>();
        TargetFollowComponent &target_follow_component = entity.component<TargetFollowComponent>();
        SceneComponent &scene_component = entity.component<SceneComponent>();

        if (!target_component.target.isValid())
        {
            stopFollow(entity);
            continue;
        }

        if (!target_follow_component.follow)
            continue;

        velocity_component.velocity = {0.0f, 0.0f};

        if (state_component.state != entity_state::IDLE
            && state_component.state != entity_state::MOVE)
            continue;

        float old_following = target_follow_component.following;

        TransformComponent &target_transform_component
            = target_component.target.component<TransformComponent>();
        SceneComponent &target_scene_component
            = target_component.target.component<SceneComponent>();

        PathFinder::Cell *target_cell
            = m_path_finder->grid().cell(target_scene_component.path_finder_bounds.getPosition());

        if (target_cell && target_cell->weight == 0)
        {
            float dist = vector2::distance(
                transform_component.transform.getPosition(),
                target_transform_component.transform.getPosition());

            if (!target_follow_component.following
                && dist > target_follow_component.min_distance * 1.5
                && dist < target_follow_component.max_distance)
                target_follow_component.following = true;

            if (target_follow_component.following && dist > target_follow_component.min_distance
                && dist < target_follow_component.max_distance)
            {
                target_follow_component.path = m_path_finder->findPath(
                    scene_component.path_finder_bounds.getPosition(),
                    target_scene_component.path_finder_bounds.getPosition());

                if (!target_follow_component.path.empty())
                {
                    int32_t point_index = target_follow_component.path.size() == 1
                        ? 0
                        : target_follow_component.path.size() - 2;

                    sf::Vector2f point = sf::Vector2f{
                        sf::Vector2i{
                            target_follow_component.path[point_index].x
                                * m_path_finder->grid().cellSize().x,
                            target_follow_component.path[point_index].y
                                * m_path_finder->grid().cellSize().y}
                        + m_path_finder->grid().cellSize() / 2};

                    float angle
                        = vector2::angleTo(transform_component.transform.getPosition(), point);

                    sf::Vector2f velocity(
                        -velocity_component.max_velocity.x * std::cos(angle),
                        -velocity_component.max_velocity.y * std::sin(angle));

                    velocity_component.velocity = velocity;

                    target_follow_component.state = TargetFollowComponent::FOLLOW;
                }
            }
            else
            {
                if (target_follow_component.following)
                {
                    target_follow_component.path.clear();
                    target_follow_component.following = false;

                    if (dist < target_follow_component.min_distance)
                        target_follow_component.state = TargetFollowComponent::RICHED;

                    if (dist > target_follow_component.max_distance)
                        target_follow_component.state = TargetFollowComponent::LOST;
                }
            }
        }
        else
        {
            if (target_follow_component.following)
            {
                target_follow_component.path.clear();
                target_follow_component.following = false;
                target_follow_component.state = TargetFollowComponent::LOST;
            }
        }

        if (old_following != target_follow_component.following)
        {
            if (velocity_component.velocity.x != 0 || velocity_component.velocity.y != 0)
                EntityUtils::setEntityState(entity, entity_state::MOVE);
            else
                EntityUtils::setEntityState(entity, entity_state::IDLE);
        }

        if (target_follow_component.following)
        {
            if (target_transform_component.transform.getPosition().x
                > transform_component.transform.getPosition().x)
                EntityUtils::setEntityDirection(entity, entity_state::RIGHT);
            else if (
                target_transform_component.transform.getPosition().x
                < transform_component.transform.getPosition().x)
                EntityUtils::setEntityDirection(entity, entity_state::LEFT);
        }
    }
}

void TargetFollowSystem::startFollow(Entity &entity)
{
    TargetComponent &target_component = entity.component<TargetComponent>();
    TargetFollowComponent &target_follow_component = entity.component<TargetFollowComponent>();
    if (target_component.target.isValid())
    {
        target_follow_component.follow = true;
        target_follow_component.following = false;
        target_follow_component.path.clear();
        target_follow_component.state = TargetFollowComponent::LOST;
    }
}

void TargetFollowSystem::stopFollow(Entity &entity)
{
    TargetFollowComponent &target_follow_component = entity.component<TargetFollowComponent>();
    target_follow_component.follow = false;
    target_follow_component.following = false;
    target_follow_component.path.clear();
    target_follow_component.state = TargetFollowComponent::NO_FOLLOWING;
}

} // namespace fck
