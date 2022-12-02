#include "look_around_system.h"

#include "../entity_utils.h"

#include "../fck/utilities.h"

namespace fck
{

LookAroundSystem::LookAroundSystem(b2::DynamicTree<Entity> *tree) : m_tree{tree}
{
}

void LookAroundSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        LookAroundComponent &look_around_component = entity.component<LookAroundComponent>();

        look_around_component.found_entities.clear();
        look_around_component.look_at_entities.clear();

        if (!look_around_component.enable)
            continue;

        m_tree->querry(look_around_component.global_bounds, [&](int32_t id) {
            Entity other = m_tree->userData(id);
            if (other != entity)
            {
                if (!other.hasComponent<StateComponent>() || !other.hasComponent<TypeComponent>())
                    return true;

                look_around_component.found_entities.push_back(other);

                SceneComponent &other_scene_component = other.component<SceneComponent>();
                if (look_around_component.global_look_bounds
                        .findIntersection(other_scene_component.global_bounds)
                        .has_value())
                    look_around_component.look_at_entities.push_back(other);
            }

            return true;
        });
    }
}

void LookAroundSystem::updateBounds(const Entity &entity)
{
    LookAroundComponent &look_around_component = entity.component<LookAroundComponent>();
    StateComponent &state_component = entity.component<StateComponent>();
    TransformComponent &transform_component = entity.component<TransformComponent>();

    sf::Vector2f global_position = transform_component.transform.getPosition();

    look_around_component.global_bounds
        = {sf::Vector2f(
               global_position.x - look_around_component.distance,
               global_position.y - look_around_component.distance),
           sf::Vector2f(look_around_component.distance * 2, look_around_component.distance * 2)};

    look_around_component.global_look_bounds
        = {sf::Vector2f(
               global_position.x
                   - (look_around_component.distance
                      * (state_component.direction == entity_state::LEFT ? 1.0f : 0.0f)),
               global_position.y - look_around_component.distance / 2),
           sf::Vector2f(look_around_component.distance, look_around_component.distance)};
}

} // namespace fck
