#include "debug_draw.h"

#include "components/components.h"

#include "fck/common.h"
#include "fck/resource_cache.h"
#include "fck/utilities.h"

namespace fck::debug_draw
{

void drawDrawableBounds(
    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.has<component::Drawable>())
        return;

    component::Drawable &drawable_component = entity.get<component::Drawable>();

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
    if (!entity.has<component::Scene>())
        return;

    component::Scene &scene_component = entity.get<component::Scene>();

    if (scene_component.tree_id < 0)
        return;

    b2::AABB aabb = scene_component.tree->getFatAABB(scene_component.tree_id);

    sf::RectangleShape rectangle(aabb.lower_bound);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Cyan);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(aabb.upper_bound - aabb.lower_bound);
    target.draw(rectangle, states);
}

void drawSceneBounds(const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.has<component::Scene>())
        return;

    component::Scene &scene_component = entity.get<component::Scene>();

    sf::RectangleShape rectangle(scene_component.global_bounds.getSize());
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(scene_component.global_bounds.getPosition());
    target.draw(rectangle, states);
}

void drawVelocity(const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.has<component::Velocity>() || !entity.has<component::Transform>())
        return;

    component::Velocity &velocity_component = entity.get<component::Velocity>();
    component::Transform &transform_component = entity.get<component::Transform>();

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

void drawMapWalls(map::Map *map, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (map->getCurrentChunkCoords().x < 0 || map->getCurrentChunkCoords().y < 0)
        return;

    const map::Chunk *chunk = map->getChunks().getData(map->getCurrentChunkCoords());
    if (!chunk)
        return;

    for (int32_t i = 0; i < chunk->getWalls().getSize(); ++i)
    {
        auto coords = chunk->getWalls().transformIndex(i);

        sf::RectangleShape rectangle(sf::Vector2f{chunk->getWallSize()});
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineColor(sf::Color::Cyan);
        rectangle.setOutlineThickness(0.3f);
        rectangle.setPosition(sf::Vector2f{vector2::mult(chunk->getWallSize(), coords)});
        target.draw(rectangle, states);
    }
}

void drawTargetFollowPath(
    const Entity &entity, map::Map *map, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.has<component::TargetFollow>())
        return;

    if (map->getCurrentChunkCoords().x < 0 || map->getCurrentChunkCoords().y < 0)
        return;

    const map::Chunk *chunk = map->getChunks().getData(map->getCurrentChunkCoords());
    if (!chunk)
        return;

    component::TargetFollow &target_follow_component = entity.get<component::TargetFollow>();

    for (const sf::Vector2i &point : target_follow_component.path)
    {
        sf::RectangleShape rectangle(sf::Vector2f{chunk->getWallSize()});
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineColor(sf::Color::Blue);
        rectangle.setOutlineThickness(0.5f);
        rectangle.setPosition(sf::Vector2f{vector2::mult(chunk->getWallSize(), point)});

        target.draw(rectangle, states);
    }
}

void drawLookAroundBound(
    const Entity &entity, sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!entity.has<component::LookAround>())
        return;

    component::LookAround &look_around_component = entity.get<component::LookAround>();

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
    if (!entity.has<component::LookAround>())
        return;

    component::LookAround &look_around_component = entity.get<component::LookAround>();

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
