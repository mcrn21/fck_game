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
            target_follow_component.follow = false;
            target_follow_component.state = TargetFollowComponent::NO_FOLLOWING;
            continue;
        }

        if (!target_follow_component.follow)
        {
            target_follow_component.state = TargetFollowComponent::NO_FOLLOWING;
            continue;
        }

        //        if (target_component.target.component<StateComponent>().state == entity_state::DEATH)
        //        {
        //            target_follow_component.follow = false;
        //            target_follow_component.state = TargetFollowComponent::NO_FOLLOWING;
        //            continue;
        //        }

        velocity_component.velocity = {0.0f, 0.0f};

        if (entity_state::NOT_AVALIBLE & state_component.state)
            continue;

        TargetFollowComponent::State old_state = target_follow_component.state;

        TransformComponent &target_transform_component
            = target_component.target.component<TransformComponent>();

        PathFinder::Cell *target_cell = m_path_finder->grid().cellByPosition(
            target_transform_component.transform.getPosition());

        if (target_cell && target_cell->weight == 0)
        {
            float dist = vector2::distance(
                transform_component.transform.getPosition(),
                target_transform_component.transform.getPosition());

            if (target_follow_component.state != TargetFollowComponent::FOLLOW
                && dist > target_follow_component.min_distance * 1.5
                && dist < target_follow_component.max_distance)
                target_follow_component.state = TargetFollowComponent::FOLLOW;

            if (target_follow_component.state == TargetFollowComponent::FOLLOW
                && dist > target_follow_component.min_distance
                && dist < target_follow_component.max_distance)
            {
                target_follow_component.path = m_path_finder->findPath(
                    m_path_finder->grid().transformPosition(
                        transform_component.transform.getPosition()),
                    m_path_finder->grid().transformPosition(
                        target_transform_component.transform.getPosition()));

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
                }
            }
            else
            {
                if (target_follow_component.state == TargetFollowComponent::FOLLOW)
                {
                    target_follow_component.path.clear();

                    if (dist < target_follow_component.min_distance)
                        target_follow_component.state = TargetFollowComponent::RICHED;

                    if (dist > target_follow_component.max_distance)
                        target_follow_component.state = TargetFollowComponent::LOST;
                }
            }
        }
        else
        {
            if (target_follow_component.state == TargetFollowComponent::FOLLOW)
            {
                target_follow_component.path.clear();
                target_follow_component.state = TargetFollowComponent::LOST;
            }
        }

        if (old_state != target_follow_component.state)
        {
            if (velocity_component.velocity.x != 0 || velocity_component.velocity.y != 0)
                entity::set_state.emit(entity, entity_state::MOVE);
            else
                entity::set_state.emit(entity, entity_state::IDLE);
        }

        if (target_follow_component.state == TargetFollowComponent::FOLLOW)
        {
            if (target_transform_component.transform.getPosition().x
                > transform_component.transform.getPosition().x)
                entity::set_direction.emit(entity, entity_state::RIGHT);
            else if (
                target_transform_component.transform.getPosition().x
                < transform_component.transform.getPosition().x)
                entity::set_direction.emit(entity, entity_state::LEFT);
        }
    }
}

} // namespace fck
