#ifndef ENTITYFUNCS_FLAADMBKTWQG_H
#define ENTITYFUNCS_FLAADMBKTWQG_H

#include "fck/entity.h"
#include "fck_common.h"
#include "sigslot/signal.hpp"
#include "skills/skill.h"

#include <SFML/Graphics.hpp>

namespace fck
{

struct entity_funcs
{
    // funcs
    // transform
    static void move(const Entity &entity, const sf::Vector2f &offset);
    static void setPosition(const Entity &entity, const sf::Vector2f &position);
    static void setParent(const Entity &entity, const Entity &parent);

    // state
    static void setState(const Entity &entity, entity_state::State state);
    static void setDirection(const Entity &entity, entity_state::Direction direction);

    // target
    static void setTarget(const Entity &entity, const Entity &target);

    // marker
    static void setMarker(const Entity &entity, const Entity &marker);

    // drawable state
    static void setDrawableState(const Entity &entity, const std::string &state);

    // sounds
    static void playSound(const Entity &entity, const std::string &sound_name);
    static void stopSound(const Entity &entity, const std::string &sound_name);
    static void stopAllSound(const Entity &entity);

    // script
    static void setScript(const Entity &entity, const std::string &script_name);

    // signals
    static sigslot::signal<const Entity &, const sf::Vector2f &> moved;
    static sigslot::signal<const Entity &, const Entity &> parent_changed;
    static sigslot::signal<const Entity &, entity_state::State> state_changed;
    static sigslot::signal<const Entity &, entity_state::Direction> direction_changed;
    static sigslot::signal<const Entity &, const Entity &> collided;
    static sigslot::signal<const Entity &, const Entity &, const Entity &> target_changed;
    static sigslot::signal<const Entity &, const Entity &> marker_changed;
    static sigslot::signal<const Entity &, const std::string &> drawable_state_changed;
    static sigslot::signal<const Entity &, float> health_changed;
    static sigslot::signal<const Entity &, float> armor_changed;
    static sigslot::signal<const Entity &, const std::string &> sound_playing;
    static sigslot::signal<const Entity &, const std::string &> sound_stopped;
    static sigslot::signal<const Entity &> all_sound_stopped;
    static sigslot::signal<const Entity &, skill::Skill *> skill_applied;
    static sigslot::signal<const Entity &, skill::Skill *> skill_finished;

private:
    entity_funcs() = delete;
    ~entity_funcs() = delete;
};

} // namespace fck

#endif // ENTITYFUNCS_FLAADMBKTWQG_H
