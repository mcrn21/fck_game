#include "components.h"
#include "../knowledge_base/drawable_factory.h"

namespace fck::component
{

// TRANSFORM
void TransformComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Transform>();

    if (component_table.contains("position"))
        component.transform.setPosition(
            vector2::tomlArrayToVector2f(component_table.at("position").as_array()));

    if (component_table.contains("rotation"))
        component.transform.setRotation(
            sf::degrees(component_table.at("rotation").as_floating_point()->get()));

    if (component_table.contains("scale"))
        component.transform.setScale(
            vector2::tomlArrayToVector2f(component_table.at("scale").as_array()));

    if (component_table.contains("origin"))
        component.transform.setScale(
            vector2::tomlArrayToVector2f(component_table.at("origin").as_array()));
}

// VELOCITY
void VelocityComponentFactory::create(Entity &entity)

{
    component::Velocity &component = entity.add<Velocity>();

    if (component_table.contains("max_velocity"))
        component.max_velocity
            = vector2::tomlArrayToVector2f(component_table.at("max_velocity").as_array());
}

// SCENE
void SceneComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Scene>();

    if (component_table.contains("local_bounds"))
        component.local_bounds
            = rect::tomlArrayToFloatRect(component_table.at("local_bounds").as_array());

    if (component_table.contains("path_finder_wall"))
        component.path_finder_wall = component_table.at("path_finder_wall").as_boolean()->get();
}

// PLAYER
void PlayerComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Player>();
}

// DRAWABLE
void DrawableComponentFactory::create(Entity &entity)
{
    auto &drawable_component = entity.add<Drawable>();

    for (const auto &it : component_table)
    {
        if (it.second.is_table())
        {
            std::string table_name = it.first.data();
            if (table_name == "shadow")
            {
                toml::table *shadow_table = it.second.as_table();
                std::string type = shadow_table->at("type").as_string()->get();

                if (type == "rect")
                {
                    sf::RectangleShape *shape = new sf::RectangleShape();
                    shape->setFillColor(sf::Color(0, 0, 0, 110));

                    shape->setSize(
                        vector2::tomlArrayToVector2f(shadow_table->at("rect_size").as_array()));

                    drawable_component.shadow_shape.reset(shape);
                }
                else if (type == "circle")
                {
                    sf::CircleShape *shape = new sf::CircleShape();
                    shape->setFillColor(sf::Color(0, 0, 0, 110));

                    shape->setPointCount(12);
                    shape->setRadius(shadow_table->at("radius").as_floating_point()->get());

                    drawable_component.shadow_shape.reset(shape);
                }

                if (shadow_table->contains("position"))
                    drawable_component.shadow_shape->setPosition(
                        vector2::tomlArrayToVector2f(shadow_table->at("position").as_array()));

                if (shadow_table->contains("rotation"))
                    drawable_component.shadow_shape->setRotation(
                        sf::degrees(shadow_table->at("rotation").as_floating_point()->get()));

                if (shadow_table->contains("scale"))
                    drawable_component.shadow_shape->setScale(
                        vector2::tomlArrayToVector2f(shadow_table->at("scale").as_array()));

                if (shadow_table->contains("origin"))
                    drawable_component.shadow_shape->setOrigin(
                        vector2::tomlArrayToVector2f(shadow_table->at("origin").as_array()));
            }
            else
            {
                auto [drawable_proxy, drawable_state, drawable_animation]
                    = DrawableFactory::createDrawable(
                        drawable_type::fromString(it.first.data()), it.second.as_table());

                drawable_component.proxy.reset(drawable_proxy);

                if (drawable_state)
                {
                    auto &drawable_state_component = entity.add<DrawableState>();
                    drawable_state_component.state.reset(drawable_state);
                }

                if (drawable_animation)
                {
                    auto &drawable_animation_component = entity.add<DrawableAnimation>();
                    drawable_animation_component.animation.reset(drawable_animation);
                }
            }
        }
        else
        {
            std::string field_name = it.first.data();

            if (field_name == "z_order")
                drawable_component.z_order = it.second.as_integer()->get();

            if (field_name == "z_order_fill_y_coordinate")
                drawable_component.z_order_fill_y_coordinate = it.second.as_boolean()->get();
        }
    }
}

// COLLISION
void CollisionComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Collision>();

    if (component_table.contains("wall"))
        component.wall = component_table.at("wall").as_boolean()->get();
}

// STATE
void StateComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<State>();

    if (component_table.contains("state"))
        component.state
            = entity_state::stateFromString(component_table.at("state").as_string()->get());

    if (component_table.contains("direction"))
        component.direction
            = entity_state::directionFromString(component_table.at("direction").as_string()->get());
}

// SOUND
void SoundComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Sound>();
}

// SCRIPT
void ScriptComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Script>();

    if (component_table.contains("script"))
    {
        std::string script_name = component_table.at("script").as_string()->get();

        script::Script *script = ScriptFactory::createScript(script_name);
        if (!script)
        {
            spdlog::warn("Script not found: {}", script_name);
            return;
        }

        component.script.reset(script);
        component.script->setEntityToTable(entity);
    }
}

// TARGET
void TargetComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Target>();
}

// TARGET_FOLLOW
void TargetFollowComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<TargetFollow>();

    if (component_table.contains("min_distance"))
        component.min_distance = component_table.at("min_distance").as_floating_point()->get();
}

// LOOK_AROUND
void LookAroundComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<LookAround>();

    if (component_table.contains("enable"))
        component.enable = component_table.at("enable").as_boolean()->get();

    if (component_table.contains("distance"))
        component.distance = component_table.at("distance").as_floating_point()->get();
}

// STATS
void StatsComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Stats>();

    if (component_table.contains("health"))
        component.health = component_table.at("health").as_floating_point()->get();

    if (component_table.contains("max_health"))
        component.max_health = component_table.at("max_health").as_floating_point()->get();

    if (component_table.contains("armor"))
        component.armor = component_table.at("armor").as_floating_point()->get();

    if (component_table.contains("max_armor"))
        component.max_armor = component_table.at("max_armor").as_floating_point()->get();

    entity.add<Damage>();
}

// SKILLS
void SkillsComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Skills>();

    std::vector<std::string> skills;
    if (component_table.contains("skills"))
        skills = vector::tomlArrayToStringVector(component_table.at("skills").as_array());

    for (const std::string &skill : skills)
        component.skills.push_back(
            std::unique_ptr<SkillBase>(KnowledgeBase::getSkill(skill)->create()));
}

// MARKER
void MarkerComponentFactory::create(Entity &entity)
{
    auto &component = entity.add<Marker>();
}

} // namespace fck::component
