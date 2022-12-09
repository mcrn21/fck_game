#include "scene.h"

namespace fck::system
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
                {
                    --cell_info->weight;
                    if (cell_info->weight < 0)
                        cell_info->weight = 0;
                }
            }
        }
    }
}

Scene::Scene(b2::DynamicTree<Entity> *tree, PathFinder *path_finder)
    : m_tree{tree}, m_path_finder{path_finder}
{
}

void Scene::moveEntity(const Entity &entity, const sf::Vector2f &offset)
{
    component::Scene &scene_component = entity.component<component::Scene>();
    component::Transform &transform_component
        = entity.component<component::Transform>();

    sf::FloatRect old_scene_global_bounds = scene_component.global_bounds;
    scene_component.global_bounds
        = transform_component.transform.getTransform().transformRect(scene_component.local_bounds);

    if (scene_component.tree_id > -1)
        m_tree->moveProxy(scene_component.tree_id, scene_component.global_bounds, offset);

    if (scene_component.path_finder_wall)
    {
        updateCellWeight(m_path_finder, sf::IntRect{old_scene_global_bounds}, true);
        updateCellWeight(
            m_path_finder,
            gridBoundsBySceneBounds(
                scene_component.global_bounds, m_path_finder->grid().cellSize()),
            false);
    }
}

void Scene::onEntityAdded(Entity &entity)
{
    component::Transform &transform_component
        = entity.component<component::Transform>();
    component::Scene &scene_component = entity.component<component::Scene>();

    scene_component.global_bounds
        = transform_component.transform.getTransform().transformRect(scene_component.local_bounds);

    scene_component.tree_id = m_tree->createProxy(scene_component.global_bounds, entity);
    scene_component.tree = m_tree;

    scene_component.path_finder = m_path_finder;

    if (scene_component.path_finder_wall)
    {
        updateCellWeight(
            m_path_finder,
            gridBoundsBySceneBounds(
                scene_component.global_bounds, m_path_finder->grid().cellSize()),
            false);
    }
}

void Scene::onEntityRemoved(Entity &entity)
{
    component::Scene &scene_component = entity.component<component::Scene>();

    m_tree->destroyProxy(scene_component.tree_id);

    if (scene_component.path_finder_wall)
    {
        updateCellWeight(
            m_path_finder,
            gridBoundsBySceneBounds(
                scene_component.global_bounds, m_path_finder->grid().cellSize()),
            true);
    }
}

} // namespace fck::system
