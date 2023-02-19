#include "../components/components.h"
#include "bindings.h"

namespace fck
{

void bindComponentsToLua(sol::state &sol_state, sol::usertype<Entity> &entity_bind)
{
    sol::table fck_namespace = sol_state["fck"].get_or_create<sol::table>();
    sol::table component_namespace = fck_namespace["component"].get_or_create<sol::table>();

    sol::usertype<component::Transform> transform_bind
        = component_namespace.new_usertype<component::Transform>(
            "Transform",
            sol::constructors<component::Transform()>(),
            "transform",
            &component::Transform::transform,
            "parent",
            &component::Transform::parent,
            "children",
            &component::Transform::children);

    entity_bind["addTransformComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Transform>()); };
    entity_bind["getTransformComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Transform>()); };
    entity_bind["removeTransformComponent"]
        = [](Entity &entity) { entity.remove<component::Transform>(); };
    entity_bind["hasTransformComponent"]
        = [](Entity &entity) { return entity.has<component::Transform>(); };

    sol::usertype<component::Velocity> velocity_bind
        = component_namespace.new_usertype<component::Velocity>(
            "Velocity",
            sol::constructors<component::Velocity()>(),
            "velocity",
            &component::Velocity::velocity,
            "max_velocity",
            &component::Velocity::max_velocity);

    entity_bind["addVelocityComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Velocity>()); };
    entity_bind["getVelocityComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Velocity>()); };
    entity_bind["removeVelocityComponent"]
        = [](Entity &entity) { entity.remove<component::Velocity>(); };
    entity_bind["hasVelocityComponent"]
        = [](Entity &entity) { return entity.has<component::Velocity>(); };

    sol::usertype<component::Scene> scene_bind = component_namespace.new_usertype<component::Scene>(
        "Scene",
        sol::constructors<component::Scene()>(),
        "local_bounds",
        &component::Scene::local_bounds,
        "global_bounds",
        &component::Scene::global_bounds,
        "tree_id",
        &component::Scene::tree_id,
        "tree",
        &component::Scene::tree,
        "path_finder_wall",
        &component::Scene::path_finder_wall);

    entity_bind["addSceneComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Scene>()); };
    entity_bind["getSceneComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Scene>()); };
    entity_bind["removeSceneComponent"] = [](Entity &entity) { entity.remove<component::Scene>(); };
    entity_bind["hasSceneComponent"]
        = [](Entity &entity) { return entity.has<component::Scene>(); };

    sol::usertype<component::Player> player_bind
        = component_namespace.new_usertype<component::Player>(
            "Player",
            sol::constructors<component::Player()>(),
            "need_change_target",
            &component::Player::need_change_target,
            "view_hard_set_position",
            &component::Player::view_hard_set_position);

    entity_bind["addPlayerComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Player>()); };
    entity_bind["getPlayerComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Player>()); };
    entity_bind["removePlayerComponent"]
        = [](Entity &entity) { entity.remove<component::Player>(); };
    entity_bind["hasPlayerComponent"]
        = [](Entity &entity) { return entity.has<component::Player>(); };

    sol::usertype<component::Drawable> drawable_bind
        = component_namespace.new_usertype<component::Drawable>(
            "Drawable",
            sol::constructors<component::Drawable()>(),
            "global_bounds",
            &component::Drawable::global_bounds,
            "z_order",
            &component::Drawable::z_order,
            "z_order_fill_y_coordinate",
            &component::Drawable::z_order_fill_y_coordinate,
            "tree_id",
            &component::Drawable::tree_id,
            "tree",
            &component::Drawable::tree,
            "proxy",
            sol::readonly_property([](component::Drawable &c) { return std::ref(c.proxy); }));

    entity_bind["addDrawableComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Drawable>()); };
    entity_bind["getDrawableComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Drawable>()); };
    entity_bind["removeDrawableComponent"]
        = [](Entity &entity) { entity.remove<component::Drawable>(); };
    entity_bind["hasDrawableComponent"]
        = [](Entity &entity) { return entity.has<component::Drawable>(); };

    sol::usertype<component::DrawableState> drawable_state_bind
        = component_namespace.new_usertype<component::DrawableState>(
            "DrawableState",
            sol::constructors<component::DrawableState()>(),
            "state",
            sol::readonly_property([](component::DrawableState &c) { return std::ref(c.state); }));

    entity_bind["addDrawableStateComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::DrawableState>()); };
    entity_bind["getDrawableStateComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::DrawableState>()); };
    entity_bind["removeDrawableStateComponent"]
        = [](Entity &entity) { entity.remove<component::DrawableState>(); };
    entity_bind["hasDrawableStateComponent"]
        = [](Entity &entity) { return entity.has<component::DrawableState>(); };

    sol::usertype<component::DrawableAnimation> drawable_animation_bind
        = component_namespace.new_usertype<component::DrawableAnimation>(
            "DrawableAnimation",
            sol::constructors<component::DrawableAnimation()>(),
            "animation",
            sol::readonly_property(
                [](component::DrawableAnimation &c) { return std::ref(c.animation); }));

    entity_bind["addDrawableAnimationComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::DrawableAnimation>()); };
    entity_bind["getDrawableAnimationComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::DrawableAnimation>()); };
    entity_bind["removeDrawableAnimationComponent"]
        = [](Entity &entity) { entity.remove<component::DrawableAnimation>(); };
    entity_bind["hasDrawableAnimationComponent"]
        = [](Entity &entity) { return entity.has<component::DrawableAnimation>(); };

    sol::usertype<component::Collision> collision_bind
        = component_namespace.new_usertype<component::Collision>(
            "Collision",
            sol::constructors<component::Collision()>(),
            "wall",
            &component::Collision::wall);

    entity_bind["addCollisionComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Collision>()); };
    entity_bind["getCollisionComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Collision>()); };
    entity_bind["removeCollisionComponent"]
        = [](Entity &entity) { entity.remove<component::Collision>(); };
    entity_bind["hasCollisionComponent"]
        = [](Entity &entity) { return entity.has<component::Collision>(); };

    sol::usertype<component::State> state_bind = component_namespace.new_usertype<component::State>(
        "State",
        sol::constructors<component::State()>(),
        "state",
        &component::State::state,
        "direction",
        &component::State::direction);

    entity_bind["addStateComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::State>()); };
    entity_bind["getStateComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::State>()); };
    entity_bind["removeStateComponent"] = [](Entity &entity) { entity.remove<component::State>(); };
    entity_bind["hasStateComponent"]
        = [](Entity &entity) { return entity.has<component::State>(); };

    sol::usertype<component::Sound> sound_bind = component_namespace.new_usertype<component::Sound>(
        "Sound",
        sol::constructors<component::Sound()>(),
        "tile_material",
        &component::Sound::tile_material,
        "groups",
        &component::Sound::groups);

    sol::usertype<component::Sound::Group> sound_group_bind
        = component_namespace.new_usertype<component::Sound::Group>(
            "SoundGroup",
            sol::constructors<component::Sound::Group()>(),
            "sound",
            &component::Sound::Group::sound,
            "sounds",
            &component::Sound::Group::sounds);

    entity_bind["addSoundComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Sound>()); };
    entity_bind["getSoundComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Sound>()); };
    entity_bind["removeSoundComponent"] = [](Entity &entity) { entity.remove<component::Sound>(); };
    entity_bind["hasSoundComponent"]
        = [](Entity &entity) { return entity.has<component::Sound>(); };

    sol::usertype<component::Script> script_bind
        = component_namespace.new_usertype<component::Script>(
            "Script",
            sol::constructors<component::Script()>(),
            "script",
            sol::readonly_property([](component::Script &c) { return std::ref(c.script); }));

    entity_bind["addScriptComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Script>()); };
    entity_bind["getScriptComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Script>()); };
    entity_bind["removeScriptComponent"]
        = [](Entity &entity) { entity.remove<component::Script>(); };
    entity_bind["hasScriptComponent"]
        = [](Entity &entity) { return entity.has<component::Script>(); };

    sol::usertype<component::Target> target_bind
        = component_namespace.new_usertype<component::Target>(
            "Target",
            sol::constructors<component::Target()>(),
            "target",
            &component::Target::target,
            "lookings",
            &component::Target::lookings);

    entity_bind["addTargetComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Target>()); };
    entity_bind["getTargetComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Target>()); };
    entity_bind["removeTargetComponent"]
        = [](Entity &entity) { entity.remove<component::Target>(); };
    entity_bind["hasTargetComponent"]
        = [](Entity &entity) { return entity.has<component::Target>(); };

    component_namespace.new_enum(
        "TargetFollowState",
        "LOST",
        component::TargetFollow::LOST,
        "RICHED",
        component::TargetFollow::RICHED);

    sol::usertype<component::TargetFollow> target_follow_bind
        = component_namespace.new_usertype<component::TargetFollow>(
            "TargetFollow",
            sol::constructors<component::TargetFollow()>(),
            "follow",
            &component::TargetFollow::follow,
            "min_distance",
            &component::TargetFollow::min_distance,
            "state",
            &component::TargetFollow::state,
            "path",
            &component::TargetFollow::path,
            "target",
            &component::TargetFollow::target);

    entity_bind["addTargetFollowComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::TargetFollow>()); };
    entity_bind["getTargetFollowComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::TargetFollow>()); };
    entity_bind["removeTargetFollowComponent"]
        = [](Entity &entity) { entity.remove<component::TargetFollow>(); };
    entity_bind["hasTargetFollowComponent"]
        = [](Entity &entity) { return entity.has<component::TargetFollow>(); };

    sol::usertype<component::LookAround> look_around_bind
        = component_namespace.new_usertype<component::LookAround>(
            "LookAround",
            sol::constructors<component::LookAround()>(),
            "enable",
            &component::LookAround::enable,
            "distance",
            &component::LookAround::distance,
            "global_bounds",
            &component::LookAround::global_bounds,
            "global_look_bounds",
            &component::LookAround::global_look_bounds,
            "found_entities",
            &component::LookAround::found_entities,
            "look_at_entities",
            &component::LookAround::look_at_entities);

    entity_bind["addLookAroundComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::LookAround>()); };
    entity_bind["getLookAroundComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::LookAround>()); };
    entity_bind["removeLookAroundComponent"]
        = [](Entity &entity) { entity.remove<component::LookAround>(); };
    entity_bind["hasLookAroundComponent"]
        = [](Entity &entity) { return entity.has<component::LookAround>(); };

    sol::usertype<component::Stats> stats_bind = component_namespace.new_usertype<component::Stats>(
        "Stats",
        sol::constructors<component::Stats()>(),
        "health",
        &component::Stats::health,
        "max_health",
        &component::Stats::max_health,
        "armor",
        &component::Stats::armor,
        "max_armor",
        &component::Stats::max_armor,
        "damage",
        &component::Stats::damage,
        "death_time",
        &component::Stats::death_time,
        "disappearance_time",
        &component::Stats::disappearance_time,
        "death_elipsed",
        &component::Stats::health);

    entity_bind["addStatsComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Stats>()); };
    entity_bind["getStatsComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Stats>()); };
    entity_bind["removeStatsComponent"] = [](Entity &entity) { entity.remove<component::Stats>(); };
    entity_bind["hasStatsComponent"]
        = [](Entity &entity) { return entity.has<component::Stats>(); };

    sol::usertype<component::Skills> skills_bind
        = component_namespace.new_usertype<component::Skills>(
            "Skills",
            sol::constructors<component::Skills()>(),
            "next_skill",
            &component::Skills::next_skill,
            "skills",
            sol::readonly_property([](component::Skills &c) { return std::ref(c.skills); }));

    entity_bind["addSkillsComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Skills>()); };
    entity_bind["getSkillsComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Skills>()); };
    entity_bind["removeSkillsComponent"]
        = [](Entity &entity) { entity.remove<component::Skills>(); };
    entity_bind["hasSkillsComponent"]
        = [](Entity &entity) { return entity.has<component::Skills>(); };

    sol::usertype<component::Damage> damage_bind
        = component_namespace.new_usertype<component::Damage>(
            "Damage",
            sol::constructors<component::Damage()>(),
            "damage",
            sol::readonly_property([](component::Damage &c) { return std::ref(c.damage); }));

    entity_bind["addDamageComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Damage>()); };
    entity_bind["getDamageComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Damage>()); };
    entity_bind["removeDamageComponent"]
        = [](Entity &entity) { entity.remove<component::Damage>(); };
    entity_bind["hasDamageComponent"]
        = [](Entity &entity) { return entity.has<component::Damage>(); };

    sol::usertype<component::Marker> marker_bind
        = component_namespace.new_usertype<component::Marker>(
            "Marker",
            sol::constructors<component::Marker()>(),
            "marker",
            &component::Marker::marker);

    entity_bind["addMarkerComponent"]
        = [](Entity &entity) { return std::ref(entity.add<component::Marker>()); };
    entity_bind["getMarkerComponent"]
        = [](Entity &entity) { return std::ref(entity.get<component::Marker>()); };
    entity_bind["removeMarkerComponent"]
        = [](Entity &entity) { entity.remove<component::Marker>(); };
    entity_bind["hasMarkerComponent"]
        = [](Entity &entity) { return entity.has<component::Marker>(); };
}

} // namespace fck
