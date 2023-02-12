#include "render.h"

#include <spdlog/spdlog.h>

namespace fck::system
{

Render::Render(b2::DynamicTree<Entity> *tree) : m_tree{tree}
{
}

void Render::moveEntity(const Entity &entity, const sf::Vector2f &offset)
{
    auto &drawable_component = entity.get<component::Drawable>();
    if (!drawable_component.proxy)
        return;

    auto &transform_component = entity.get<component::Transform>();

    if (drawable_component.z_order_fill_y_coordinate)
        drawable_component.z_order = transform_component.transform.getPosition().y + Z_ORDER;

    drawable_component.global_bounds = transform_component.transform.getTransform().transformRect(
        drawable_component.proxy->getGlobalBounds());

    if (drawable_component.tree_id > -1)
        drawable_component.tree->moveProxy(
            drawable_component.tree_id, drawable_component.global_bounds, offset);
}

void Render::onEntityAdded(Entity &entity)
{
    auto &transform_component = entity.get<component::Transform>();
    auto &drawable_component = entity.get<component::Drawable>();

    drawable_component.global_bounds = transform_component.transform.getTransform().transformRect(
        drawable_component.proxy->getGlobalBounds());

    drawable_component.tree_id = m_tree->createProxy(drawable_component.global_bounds, entity);
    drawable_component.tree = m_tree;
}

void Render::onEntityRemoved(Entity &entity)
{
    auto &drawable_component = entity.get<component::Drawable>();

    m_tree->destroyProxy(drawable_component.tree_id);
    drawable_component.tree_id = -1;
}

} // namespace fck::system
