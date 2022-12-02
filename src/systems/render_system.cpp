#include "render_system.h"

#include <spdlog/spdlog.h>

namespace fck
{

RenderSystem::RenderSystem(b2::DynamicTree<Entity> *tree) : m_tree{tree}
{
}

void RenderSystem::moveEntity(const Entity &entity, const sf::Vector2f &offset)
{
    DrawableComponent &drawable_component = entity.component<DrawableComponent>();
    TransformComponent &transform_component = entity.component<TransformComponent>();

    if (drawable_component.z_order_fill_y_coordinate)
        drawable_component.z_order = transform_component.transform.getPosition().y + Z_ORDER;

    drawable_component.global_bounds = transform_component.transform.getTransform().transformRect(
        drawable_component.drawable->localBounds());

    drawable_component.global_content_bounds
        = transform_component.transform.getTransform().transformRect(
            drawable_component.drawable->contentBounds());

    if (drawable_component.tree_id > -1)
        drawable_component.tree->moveProxy(
            drawable_component.tree_id, drawable_component.global_bounds, offset);
}

void RenderSystem::onEntityAdded(Entity &entity)
{
    DrawableComponent &drawable_component = entity.component<DrawableComponent>();
    drawable_component.tree_id = m_tree->createProxy(drawable_component.global_bounds, entity);
    drawable_component.tree = m_tree;
}

void RenderSystem::onEntityRemoved(Entity &entity)
{
    DrawableComponent &drawable_component = entity.component<DrawableComponent>();
    m_tree->destroyProxy(drawable_component.tree_id);
}

} // namespace fck
