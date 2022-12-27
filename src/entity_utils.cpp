#include "entity_utils.h"

namespace fck
{

std::vector<std::string> entity::getDrawableStates(const Entity &entity)
{
    if (entity.has<component::DrawableAnimation>())
    {
        return entity.get<component::DrawableAnimation>().animation->getStates();
    }
    else if (entity.has<component::DrawableState>())
    {
        return entity.get<component::DrawableState>().state->getStates();
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

// sound
Signal<const Entity &, const std::string &> entity::play_sound;
Signal<const Entity &, const std::string &> entity::stop_sound;
Signal<const Entity &> entity::stop_all_sound;

// other
Signal<const Entity &, tile_material_type::Type> entity::set_tile_material;

// skills
Signal<const Entity &, SkillBase *> entity::skill_applied;
Signal<const Entity &, SkillBase *> entity::skill_finished;

} // namespace fck
