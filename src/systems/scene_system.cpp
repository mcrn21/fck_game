#include "scene_system.h"

namespace fck
{

sf::IntRect gridBoundsBySceneBounds(
    const sf::FloatRect &scene_bounds, const sf::Vector2i &cell_size)
{
    sf::Vector2i lower_point = {int32_t(scene_bounds.left), int32_t(scene_bounds.top)};

    sf::Vector2i upper_point
        = {int32_t(scene_bounds.left + scene_bounds.width),
           int32_t(scene_bounds.top + scene_bounds.height)};

    sf::Vector2i lower_cell = {lower_point.x / cell_size.x, lower_point.y / cell_size.y};

    if (lower_point.x % cell_size.x > cell_size.x * 0.8)
        ++lower_cell.x;

    if (lower_point.y % cell_size.y > cell_size.y * 0.8)
        ++lower_cell.y;

    sf::Vector2i upper_cell = {upper_point.x / cell_size.x, upper_point.y / cell_size.y};

    if (upper_point.x % cell_size.x > cell_size.x * 0.2)
        ++upper_cell.x;

    if (upper_point.y % cell_size.y > cell_size.y * 0.2)
        ++upper_cell.y;

    sf::IntRect grid_bounds;

    grid_bounds.left = lower_cell.x;
    grid_bounds.top = lower_cell.y;

    grid_bounds.width = upper_cell.x - lower_cell.x;
    grid_bounds.height = upper_cell.y - lower_cell.y;

    if (grid_bounds.width == 0)
        ++grid_bounds.width;

    if (grid_bounds.height == 0)
        ++grid_bounds.height;

    return grid_bounds;
}

sf::IntRect gridPositionByPosition(const sf::Vector2f &position, const sf::Vector2i &cell_size)
{
    sf::IntRect grid_bounds;

    grid_bounds.left = position.x / cell_size.x;
    grid_bounds.top = position.y / cell_size.y;
    grid_bounds.width = 1;
    grid_bounds.height = 1;

    return grid_bounds;
}

void updateCellWeight(PathFinder *path_finder, const sf::IntRect &bounds, bool reduce)
{
    for (int32_t i = 0; i < bounds.height; ++i)
    {
        for (int32_t j = 0; j < bounds.width; ++j)
        {
            PathFinder::Cell *cell_info
                = path_finder->grid().cell({bounds.left + j, bounds.top + i});
            if (cell_info)
            {
                if (!reduce)
                    ++cell_info->weight;
                else
                    --cell_info->weight;
            }
        }
    }
}

SceneSystem::SceneSystem(b2::DynamicTree<Entity> *tree, PathFinder *path_finder)
    : m_tree{tree}, m_path_finder{path_finder}
{
}

void SceneSystem::moveEntity(const Entity &entity, const sf::Vector2f &offset)
{
    SceneComponent &scene_component = entity.component<SceneComponent>();
    TransformComponent &transform_component = entity.component<TransformComponent>();

    scene_component.global_bounds
        = transform_component.transform.getTransform().transformRect(scene_component.local_bounds);

    if (scene_component.tree_id > -1)
    {
        m_tree->moveProxy(scene_component.tree_id, scene_component.global_bounds, offset);
    }

    if (scene_component.wall)
    {
        updateCellWeight(m_path_finder, scene_component.path_finder_bounds, true);

        scene_component.path_finder_bounds = gridBoundsBySceneBounds(
            scene_component.global_bounds, m_path_finder->grid().cellSize());

        updateCellWeight(m_path_finder, scene_component.path_finder_bounds, false);
    }
    else
    {
        scene_component.path_finder_bounds = gridPositionByPosition(
            transform_component.transform.getPosition(), m_path_finder->grid().cellSize());
    }
}

void SceneSystem::onEntityAdded(Entity &entity)
{
    SceneComponent &scene_component = entity.component<SceneComponent>();
    scene_component.tree_id = m_tree->createProxy(scene_component.global_bounds, entity);
    scene_component.tree = m_tree;

    scene_component.path_finder = m_path_finder;

    if (scene_component.wall)
    {
        scene_component.path_finder_bounds = gridBoundsBySceneBounds(
            scene_component.global_bounds, m_path_finder->grid().cellSize());

        updateCellWeight(m_path_finder, scene_component.path_finder_bounds, false);
    }
    else
    {
        TransformComponent &transform_component = entity.component<TransformComponent>();
        scene_component.path_finder_bounds = gridPositionByPosition(
            transform_component.transform.getPosition(), m_path_finder->grid().cellSize());
    }
}

void SceneSystem::onEntityRemoved(Entity &entity)
{
    SceneComponent &scene_component = entity.component<SceneComponent>();
    m_tree->destroyProxy(scene_component.tree_id);

    if (scene_component.wall)
        updateCellWeight(m_path_finder, scene_component.path_finder_bounds, true);
}

} // namespace fck
