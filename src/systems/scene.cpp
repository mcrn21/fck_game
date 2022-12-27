#include "scene.h"

namespace fck::system
{

Scene::Scene(b2::DynamicTree<Entity> *tree) : m_tree{tree}
{
}

void Scene::moveEntity(const Entity &entity, const sf::Vector2f &offset)
{
    component::Scene &scene_component = entity.get<component::Scene>();
    component::Transform &transform_component = entity.get<component::Transform>();

    sf::FloatRect old_scene_global_bounds = scene_component.global_bounds;
    scene_component.global_bounds
        = transform_component.transform.getTransform().transformRect(scene_component.local_bounds);

    if (scene_component.tree_id > -1)
        m_tree->moveProxy(scene_component.tree_id, scene_component.global_bounds, offset);
}

void Scene::onEntityAdded(Entity &entity)
{
    component::Transform &transform_component = entity.get<component::Transform>();
    component::Scene &scene_component = entity.get<component::Scene>();

    scene_component.global_bounds
        = transform_component.transform.getTransform().transformRect(scene_component.local_bounds);

    scene_component.tree_id = m_tree->createProxy(scene_component.global_bounds, entity);
    scene_component.tree = m_tree;
}

void Scene::onEntityRemoved(Entity &entity)
{
    component::Scene &scene_component = entity.get<component::Scene>();

    m_tree->destroyProxy(scene_component.tree_id);
}

} // namespace fck::system
