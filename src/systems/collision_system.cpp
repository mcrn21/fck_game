#include "collision_system.h"

#include "../entity_utils.h"

#include "../fck/collisions.h"
#include "../fck/utilities.h"

#include "spdlog/spdlog.h"

namespace fck
{

struct Sweep
{
    void setHit(const std::optional<collisions::Hit> &new_hit)
    {
        if (new_hit && new_hit->time != 0)
        {
            if (new_hit->time <= time)
            {
                time = new_hit->time;
                hit = new_hit;
            }
        }
    }

    std::optional<collisions::Hit> hit;
    float time = 1;
    Entity entity;
};

CollisionSystem::CollisionSystem()
{
}

void CollisionSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        SceneComponent &scene_component = entity.component<SceneComponent>();
        VelocityComponent &velocity_component = entity.component<VelocityComponent>();
        TransformComponent &transform_component = entity.component<TransformComponent>();

        if (!vector2::isValid(velocity_component.velocity))
            continue;

        sf::Vector2f delta = velocity_component.velocity * float(delta_time);
        float delta_max = std::max(delta.x, delta.y);

        bool collided = false;
        sf::FloatRect global_bounds = scene_component.global_bounds;
        sf::FloatRect querry_bounds = rect::extends(
            scene_component.global_bounds, sf::Vector2f{std::abs(delta_max), std::abs(delta_max)});
        querry_bounds
            = rect::extends(querry_bounds, scene_component.global_bounds.getSize() / 2.0f);

        sf::Vector2f position = rect::center(global_bounds);
        sf::Vector2f delta_position = transform_component.transform.getPosition() - position;

        for (int32_t i = 0; i < 2; ++i)
        {
            Sweep sweep;
            int32_t foo = 1;
            scene_component.tree->querry(querry_bounds, [&](int32_t id) {
                Entity other = scene_component.tree->userData(id);
                if (other != entity)
                {
                    if (!other.hasComponent<CollisionComponent>())
                        return true;

                    collisions::AABB other_aabb{other.component<SceneComponent>().global_bounds};
                    other_aabb.half += scene_component.global_bounds.getSize() / 2.0f;

                    auto hit = other_aabb.intersectSegment(position, delta);

                    if (hit)
                    {
                        CollisionComponent &other_collision_component
                            = other.component<CollisionComponent>();
                        if (other_collision_component.type == collision_type::DYNAMIC)
                            return true;

                        sweep.setHit(hit);
                        sweep.entity = other;
                    }
                }
                return true;
            });

            if (sweep.hit)
            {
                position = sweep.hit->position + sweep.hit->normal;

                if (sweep.hit->normal.x != 0)
                {
                    collided = true;
                    position.y += delta.y;
                    delta.x = 0;
                    velocity_component.velocity.x = 0;
                }
                else if (sweep.hit->normal.y != 0)
                {
                    collided = true;
                    position.x += delta.x;
                    delta.y = 0;
                    velocity_component.velocity.y = 0;
                }

                rect::setCenter(global_bounds, position);
                rect::setCenter(querry_bounds, position);
            }
        }

        if (collided)
        {
            position -= velocity_component.velocity * float(delta_time) * 1.2f;
            position += delta_position;
            EntityUtils::setEntityPosition(entity, position);
        }
    }
}

} // namespace fck
