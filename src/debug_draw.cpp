#include "debug_draw.h"

#include "components/components.h"

#include "fck/common.h"
#include "fck/resource_cache.h"
#include "fck/utilities.h"

namespace fck::debug_draw
{

void drawEntityId(const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<SceneComponent>())
        return;

    SceneComponent &scene_component = entity.component<SceneComponent>();

    sf::Text text;
    text.setFont(*ResourceCache::resource<sf::Font>("consolate_elf"));
    text.setString(std::string("ID: ") + std::to_string(entity.id().index()));
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(10);
    text.setPosition(rect::center(scene_component.global_bounds));
    target.draw(text, states);
}

void drawDrawableBounds(
    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<DrawableComponent>())
        return;

    DrawableComponent &drawable_component = entity.component<DrawableComponent>();

    sf::RectangleShape rectangle(drawable_component.global_bounds.getSize());
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Yellow);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(drawable_component.global_bounds.getPosition());
    target.draw(rectangle, states);
}

void drawSceneTreeAABB(
    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<SceneComponent>())
        return;

    SceneComponent &scene_component = entity.component<SceneComponent>();

    if (scene_component.tree_id < 0)
        return;

    b2::AABB aabb = scene_component.tree->fatAABB(scene_component.tree_id);

    sf::RectangleShape rectangle(aabb.lower_bound);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Cyan);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(aabb.upper_bound - aabb.lower_bound);
    target.draw(rectangle, states);
}

void drawDrawableContentBounds(
    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<DrawableComponent>())
        return;

    DrawableComponent &drawable_component = entity.component<DrawableComponent>();

    sf::RectangleShape rectangle(drawable_component.global_content_bounds.getSize());
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(drawable_component.global_content_bounds.getPosition());
    target.draw(rectangle, states);
}

void drawSceneBounds(const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<SceneComponent>())
        return;

    SceneComponent &scene_component = entity.component<SceneComponent>();

    sf::RectangleShape rectangle(scene_component.global_bounds.getSize());
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(scene_component.global_bounds.getPosition());
    target.draw(rectangle, states);
}

void drawVelocity(const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<VelocityComponent>() || !entity.hasComponent<TransformComponent>())
        return;

    VelocityComponent &velocity_component = entity.component<VelocityComponent>();
    TransformComponent &transform_component = entity.component<TransformComponent>();

    float d = std::abs(vector2::distance(
        transform_component.transform.getPosition(),
        transform_component.transform.getPosition() + velocity_component.velocity));

    float angle = vector2::angleTo(
        transform_component.transform.getPosition(),
        transform_component.transform.getPosition() - velocity_component.velocity);

    sf::RectangleShape line{sf::Vector2f{d, 0.5f}};
    line.setOrigin(sf::Vector2f{0, line.getSize().y / 2});
    line.setPosition(transform_component.transform.getPosition());
    line.rotate(sf::radians(angle));

    target.draw(line, states);
}

void drawPathFinderCellsBounds(
    const Entity &entity,
    const sf::Vector2i &cell_size,
    sf::RenderTarget &target,
    const sf::RenderStates &states)
{
    if (!entity.hasComponent<SceneComponent>())
        return;

    SceneComponent &scene_component = entity.component<SceneComponent>();

    sf::RectangleShape rectangle(sf::Vector2f(
        scene_component.path_finder_bounds.width * cell_size.x,
        scene_component.path_finder_bounds.height * cell_size.y));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Cyan);
    rectangle.setOutlineThickness(0.5f);
    rectangle.setPosition(sf::Vector2f(
        scene_component.path_finder_bounds.left * cell_size.x,
        scene_component.path_finder_bounds.top * cell_size.y));

    target.draw(rectangle, states);
}

void drawTargetFollowPath(
    const Entity &entity,
    const sf::Vector2i &cell_size,
    sf::RenderTarget &target,
    const sf::RenderStates &states)
{
    if (!entity.hasComponent<TargetFollowComponent>())
        return;

    TargetFollowComponent &target_follow_component = entity.component<TargetFollowComponent>();

    for (const sf::Vector2i &point : target_follow_component.path)
    {
        sf::RectangleShape rectangle(sf::Vector2f(cell_size.x, cell_size.y));
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineColor(sf::Color::Blue);
        rectangle.setOutlineThickness(0.5f);
        rectangle.setPosition(sf::Vector2f(point.x * cell_size.x, point.y * cell_size.y));

        target.draw(rectangle, states);
    }
}

void drawLookAroundBound(
    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<LookAroundComponent>())
        return;

    LookAroundComponent &look_around_component = entity.component<LookAroundComponent>();

    sf::RectangleShape rectangle(look_around_component.global_bounds.getSize());
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(look_around_component.global_bounds.getPosition());
    target.draw(rectangle, states);
}

void drawLookAroundLookBound(
    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.hasComponent<LookAroundComponent>())
        return;

    LookAroundComponent &look_around_component = entity.component<LookAroundComponent>();

    sf::RectangleShape rectangle(look_around_component.global_look_bounds.getSize());
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(look_around_component.global_look_bounds.getPosition());
    target.draw(rectangle, states);
}

//void drawCollisionHit(
//    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
//{
//    if (!entity.hasComponent<CollisionComponent>())
//        return;

//    CollisionComponent &collision_component = entity.component<CollisionComponent>();

//    if (collision_component.hit)
//    {
//        float size = collision_component.hit->normal.x != 0 ? collision_component.aabb.half.x
//                                                            : collision_component.aabb.half.y;
//        sf::RectangleShape rectangle{{size * 2, size * 2}};
//        rectangle.setFillColor(sf::Color::Transparent);
//        rectangle.setOutlineColor(sf::Color::Yellow);
//        rectangle.setOutlineThickness(1);
//        rectangle.setOrigin({size, size});
//        rectangle.setPosition(collision_component.hit->position);

//        target.draw(rectangle, states);
//    }
//}

} // namespace fck::debug_draw
