#include "entity_utils.h"

namespace fck
{

std::vector<std::string> entity::getDrawableStates(const Entity &entity)
{
    if (entity.hasComponent<component::DrawableAnimation>())
    {
        return entity.getComponent<component::DrawableAnimation>().animation->getStates();
    }
    else if (entity.hasComponent<component::DrawableState>())
    {
        return entity.getComponent<component::DrawableState>().state->getStates();
    }

    return {};
}

// transform
Signal<const Entity &, const sf::Vector2f &> entity::move;
Signal<const Entity &, const sf::Vector2f &> entity::set_position;
Signal<const Entity &, const Entity &> entity::set_parent;
// state
Signal<const Entity &, entity_state::State> entity::set_state;
Signal<const Entity &, entity_state::Direction> entity::set_direction;
// target
Signal<const Entity &, const Entity &> entity::set_target;
// marker
Signal<const Entity &, const Entity &> entity::set_marker;
// stats
Signal<const Entity &, float> entity::set_heath;
Signal<const Entity &, float> entity::set_armor;
// destroy
Signal<const Entity &> entity::destroy;
// collided
Signal<const Entity &, const Entity &> entity::collided;
// drawable
Signal<const Entity &, const std::string &> entity::set_drawable_state;

} // namespace fck
