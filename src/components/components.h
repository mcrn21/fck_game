#ifndef COMPONENTS_QCNBORXCAKAM_H
#define COMPONENTS_QCNBORXCAKAM_H

#include "../damages/damage_base.h"
#include "../fck/b2_dynamic_tree.h"
#include "../fck/drawable_animation.h"
#include "../fck/drawable_proxy.h"
#include "../fck/drawable_state.h"
#include "../fck/entity.h"
#include "../fck_common.h"
#include "../knowledge_base/entity_factory.h"
#include "../knowledge_base/knowledge_base.h"
#include "../scripts/script.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <list>

namespace fck::component
{

// TRANSFORM
struct Transform
{
    sf::Transformable transform;

    Entity parent;
    std::vector<Entity> children;
};

struct TransformComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::TRANSFORM, TransformComponentFactory);

// VELOCITY
struct Velocity
{
    sf::Vector2f velocity;
    sf::Vector2f max_velocity;
};

struct VelocityComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::VELOCITY, VelocityComponentFactory);

// SCENE
struct Scene
{
    sf::FloatRect local_bounds;
    sf::FloatRect global_bounds;

    // scene tree
    int32_t tree_id = -1;
    b2::DynamicTree<Entity> *tree = nullptr;

    // path finding
    bool path_finder_wall = false;
};

struct SceneComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::SCENE, SceneComponentFactory);

// PLAYER
struct Player
{
    bool need_change_target = false;
    bool view_hard_set_position = false;
};

struct PlayerComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::PLAYER, PlayerComponentFactory);

// DRAWABLE
struct Drawable
{
    std::unique_ptr<DrawableProxyBase> proxy;
    sf::FloatRect global_bounds;

    int32_t z_order = 0;
    bool z_order_fill_y_coordinate = true;

    int32_t tree_id = -1;
    b2::DynamicTree<Entity> *tree = nullptr;

    std::unique_ptr<sf::Shape> shadow_shape;
};

struct DrawableComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::DRAWABLE, DrawableComponentFactory);

// DRAWABLE_STATE
struct DrawableState
{
    std::unique_ptr<fck::DrawableState> state;
};

// DRAWABLE_ANIMATION
struct DrawableAnimation
{
    std::unique_ptr<fck::DrawableAnimation> animation;
};

// COLLISION
struct Collision
{
    bool wall = false;
};

struct CollisionComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::COLLISION, CollisionComponentFactory);

// STATE
struct State
{
    entity_state::State state = entity_state::IDLE;
    entity_state::Direction direction = entity_state::RIGHT;
};

struct StateComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::STATE, StateComponentFactory);

// SOUND
struct Sound
{
    struct Group
    {
        sf::Sound *sound = nullptr;
        std::unordered_map<std::string, sf::Sound> sounds;
    };

    tile_material_type::Type tile_material = tile_material_type::NO_TYPE;
    std::unordered_map<std::string, Group> groups;
};

struct SoundComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::SOUND, SoundComponentFactory);

// SCRIPT
struct Script
{
    //    std::unique_ptr<EntityScriptBase> entity_script;
    std::unique_ptr<script::Script> script;
};

struct ScriptComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::SCRIPT, ScriptComponentFactory);

// TARGET
struct Target
{
    Entity target;
    std::vector<Entity> lookings;
};

struct TargetComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::TARGET, TargetComponentFactory);

// TARGET_FOLLOW
struct TargetFollow
{
    enum State
    {
        LOST,
        RICHED
    };

    bool follow = false;
    float min_distance = 0.0f;
    State state = LOST;
    std::vector<sf::Vector2i> path;
    sf::Vector2f target;
};

struct TargetFollowComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::TARGET_FOLLOW, TargetFollowComponentFactory);

// LOOK_AROUND
struct LookAround
{
    bool enable = true;
    float distance = 0.0f;

    sf::FloatRect global_bounds;
    sf::FloatRect global_look_bounds;

    std::list<Entity> found_entities;
    std::list<Entity> look_at_entities;
};

struct LookAroundComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::LOOK_AROUND, LookAroundComponentFactory);

// STATS
struct Stats
{
    float health = 100.0f;
    float max_health = 100.0f;

    float armor = 100.0f;
    float max_armor = 100.0f;

    float damage = 0.0f;

    double death_time = 0.4;
    double disappearance_time = 2;
    double death_elipsed = 0;
};

struct StatsComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::STATS, StatsComponentFactory);

// SKILLS
struct Skills
{
    int32_t next_skill = -1;
    std::vector<std::unique_ptr<SkillBase>> skills;
};

struct SkillsComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::SKILLS, SkillsComponentFactory);

// DAMAGE
struct Damage
{
    std::unique_ptr<DamageBase> damage;
};

// MARKER
struct Marker
{
    Entity marker;
};

struct MarkerComponentFactory : public ComponentFactory::Factory
{
    void create(Entity &entity);
};
REGISTER_COMPONENT_FACTORY(component_type::MARKER, MarkerComponentFactory);

} // namespace fck::component

#endif // COMPONENTS_QCNBORXCAKAM_H
