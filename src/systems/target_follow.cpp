#include "target_follow.h"

#include "../entity_utils.h"

#include "../fck/a_star.h"
#include "../fck/utilities.h"

namespace fck::system
{

TargetFollow::TargetFollow() : m_walls{nullptr}
{
}

const Grid<int32_t> *TargetFollow::getWalls() const
{
    return m_walls;
}

void TargetFollow::setWalls(const Grid<int32_t> *walls)
{
    m_walls = walls;
}

void TargetFollow::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::TargetFollow &target_follow_component
            = entity.getComponent<component::TargetFollow>();
        component::Transform &transform_component = entity.getComponent<component::Transform>();
        component::Velocity &velocity_component = entity.getComponent<component::Velocity>();
        component::State &state_component = entity.getComponent<component::State>();

        if (!target_follow_component.follow || !m_walls)
        {
            if (!target_follow_component.path.empty())
            {
                target_follow_component.path.clear();
                target_follow_component.state = component::TargetFollow::LOST;
                velocity_component.velocity = {0.0f, 0.0f};
                entity::set_state.emit(entity, entity_state::IDLE);
                entity::set_drawable_state.emit(
                    entity, entity_state::stateToString(entity_state::IDLE));
                entity::stop_sound.emit(entity, entity_state::stateToString(entity_state::MOVE));
            }
            continue;
        }

        float dist_to_target = vector2::distance(
            transform_component.transform.getPosition(), target_follow_component.target);

        if (target_follow_component.state == component::TargetFollow::RICHED
            && dist_to_target < target_follow_component.min_distance * 1.5f)
            continue;

        target_follow_component.state = component::TargetFollow::LOST;
        velocity_component.velocity = {0.0f, 0.0f};

        sf::Vector2i target_coord = m_walls->transformPosition(target_follow_component.target);

        // Need update path
        if ((target_follow_component.path.empty()
             || target_follow_component.path.front() != target_coord)
            && dist_to_target > target_follow_component.min_distance)
        {
            // Check cell reachable
            int32_t cell_weight = m_walls->getData(target_coord);
            if (cell_weight != 0)
            {
                static std::vector<sf::Vector2i> neighbor_coords
                    = {{-1, 0}, {0, -1}, {0, 1}, {0, 1}};
                for (const sf::Vector2i &neighbor_coord : neighbor_coords)
                {
                    const int32_t &neighbor_cell_weight
                        = m_walls->getData(target_coord + neighbor_coord);
                    if (neighbor_cell_weight == 0)
                    {
                        target_coord = target_coord + neighbor_coord;
                        cell_weight = neighbor_cell_weight;
                        break;
                    }
                }
            }

            if (cell_weight == 0)
            {
                PathFinder path_finder{*m_walls};
                target_follow_component.path = path_finder.findPath(
                    m_walls->transformPosition(transform_component.transform.getPosition()),
                    target_coord);

                if (!target_follow_component.path.empty())
                    target_follow_component.path.erase(target_follow_component.path.end() - 1);
            }
        }

        if (target_follow_component.path.empty())
        {
            entity::set_state.emit(entity, entity_state::IDLE);
            continue;
        }

        if (dist_to_target > target_follow_component.min_distance)
        {
            sf::Vector2f path_point = {-1, -1};
            while (!target_follow_component.path.empty())
            {
                sf::Vector2f next_path_point = sf::Vector2f{
                    sf::Vector2i{
                        target_follow_component.path.back().x * m_walls->getTileSize().x,
                        target_follow_component.path.back().y * m_walls->getTileSize().y}
                    + m_walls->getTileSize() / 2};

                float dist_to_next_point = vector2::distance(
                    transform_component.transform.getPosition(), next_path_point);

                if (dist_to_next_point < (m_walls->getTileSize().x / 4))
                {
                    target_follow_component.path.erase(target_follow_component.path.end() - 1);
                }
                else
                {
                    path_point = next_path_point;
                    break;
                }
            }

            if (path_point != sf::Vector2f{-1, -1})
            {
                float angle
                    = vector2::angleTo(transform_component.transform.getPosition(), path_point);

                velocity_component.velocity
                    = {-velocity_component.max_velocity.x * std::cos(angle),
                       -velocity_component.max_velocity.y * std::sin(angle)};

                if ((velocity_component.velocity.x != 0 || velocity_component.velocity.y != 0)
                    && state_component.state != entity_state::MOVE)
                {
                    entity::set_state.emit(entity, entity_state::MOVE);
                    entity::set_drawable_state.emit(
                        entity, entity_state::stateToString(entity_state::MOVE));
                    entity::play_sound.emit(
                        entity, entity_state::stateToString(entity_state::MOVE));
                }
                else if (
                    !vector2::isValid(velocity_component.velocity)
                    && state_component.state != entity_state::IDLE)
                {
                    entity::set_state.emit(entity, entity_state::IDLE);
                    entity::set_drawable_state.emit(
                        entity, entity_state::stateToString(entity_state::IDLE));
                    entity::stop_sound.emit(
                        entity, entity_state::stateToString(entity_state::MOVE));
                }

                if (velocity_component.velocity.x > 0)
                    entity::set_direction.emit(entity, entity_state::RIGHT);
                else if (velocity_component.velocity.x < 0)
                    entity::set_direction.emit(entity, entity_state::LEFT);
            }
        }
        else
        {
            target_follow_component.path.clear();
            target_follow_component.state = component::TargetFollow::RICHED;
            entity::set_state.emit(entity, entity_state::IDLE);
            entity::set_drawable_state.emit(
                entity, entity_state::stateToString(entity_state::IDLE));
            entity::stop_sound.emit(entity, entity_state::stateToString(entity_state::MOVE));
        }
    }
}

} // namespace fck::system
