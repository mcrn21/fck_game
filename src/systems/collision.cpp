#include "collision.h"

#include "../entity_utils.h"

#include "../fck/collisions.h"
#include "../fck/utilities.h"

#include "spdlog/spdlog.h"

namespace fck::system
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

Collision::Collision()
{
}

void Collision::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::Scene &scene_component = entity.getComponent<component::Scene>();
        component::Velocity &velocity_component
            = entity.getComponent<component::Velocity>();
        component::Transform &transform_component = entity.getComponent<component::Transform>();
        component::Collision &collision_component = entity.getComponent<component::Collision>();

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

        Entity prev_not_wall_collided_entity;
        for (int32_t i = 0; i < 2; ++i)
        {
            Sweep sweep;
            int32_t foo = 1;
            scene_component.tree->querry(querry_bounds, [&](int32_t id) {
                Entity other = scene_component.tree->getUserData(id);
                if (other != entity)
                {
                    if (!other.hasComponent<component::Collision>())
                        return true;

                    component::Collision &other_collision_component
                        = other.getComponent<component::Collision>();

                    collisions::AABB other_aabb{other.getComponent<component::Scene>().global_bounds};
                    other_aabb.half += scene_component.global_bounds.getSize() / 2.0f;

                    auto hit = other_aabb.intersectSegment(position, delta);

                    if (hit)
                    {
                        if (!other_collision_component.wall && hit->time != 0)
                        {
                            if (prev_not_wall_collided_entity == other)
                                return true;

                            prev_not_wall_collided_entity = other;
                            entity::collided.emit(entity, other);
                            return true;
                        }

                        sweep.setHit(hit);
                        sweep.entity = other;
                    }
                }
                return true;
            });

            if (sweep.hit)
            {
                entity::collided.emit(entity, sweep.entity);

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
            entity::set_position.emit(entity, position);
        }
    }
}

} // namespace fck::system
