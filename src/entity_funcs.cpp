#include "entity_funcs.h"

#include "components/components.h"

namespace fck
{

// signals
sigslot::signal<const Entity &, const sf::Vector2f &> entity_funcs::moved;
sigslot::signal<const Entity &, const Entity &> entity_funcs::parent_changed;
sigslot::signal<const Entity &, entity_state::State> entity_funcs::state_changed;
sigslot::signal<const Entity &, entity_state::Direction> entity_funcs::direction_changed;
sigslot::signal<const Entity &, const Entity &> entity_funcs::collided;
sigslot::signal<const Entity &, const Entity &, const Entity &> entity_funcs::target_changed;
sigslot::signal<const Entity &, const Entity &> entity_funcs::marker_changed;
sigslot::signal<const Entity &, const std::string &> entity_funcs::drawable_state_changed;
sigslot::signal<const Entity &, float> entity_funcs::health_changed;
sigslot::signal<const Entity &, float> entity_funcs::armor_changed;
sigslot::signal<const Entity &, const std::string &> entity_funcs::sound_playing;
sigslot::signal<const Entity &, const std::string &> entity_funcs::sound_stopped;
sigslot::signal<const Entity &> entity_funcs::all_sound_stopped;
sigslot::signal<const Entity &, skill::Skill *> entity_funcs::skill_applied;
sigslot::signal<const Entity &, skill::Skill *> entity_funcs::skill_finished;

// funcs
// transform
void entity_funcs::move(const Entity &entity, const sf::Vector2f &offset)
{
    auto &transform_component = entity.get<component::Transform>();
    transform_component.transform.move(offset);

    if (!transform_component.children.empty())
    {
        for (auto it = transform_component.children.begin();
             it != transform_component.children.end();)
        {
            if (it->isValid())
            {
                auto &child_transform_component = it->get<component::Transform>();
                setPosition(*it, transform_component.transform.getPosition());
                ++it;
            }
            else
            {
                it = transform_component.children.erase(it);
            }
        }
    }

    moved(entity, offset);
}

void entity_funcs::setPosition(const Entity &entity, const sf::Vector2f &position)
{
    auto &transform_component = entity.get<component::Transform>();
    move(entity, position - transform_component.transform.getPosition());
}

void entity_funcs::setParent(const Entity &entity, const Entity &parent)
{
    auto &transform_component = entity.get<component::Transform>();

    if (transform_component.parent == parent)
        return;

    if (transform_component.parent.isValid())
    {
        auto &parent_transform_component = transform_component.parent.get<component::Transform>();

        parent_transform_component.children.erase(
            std::remove(
                parent_transform_component.children.begin(),
                parent_transform_component.children.end(),
                entity),
            parent_transform_component.children.end());
    }

    transform_component.parent = parent;

    if (transform_component.parent.isValid())
    {
        auto &parent_transform_component = transform_component.parent.get<component::Transform>();
        parent_transform_component.children.push_back(entity);
        setPosition(entity, parent_transform_component.transform.getPosition());
    }

    parent_changed(entity, parent);
}

void entity_funcs::setState(const Entity &entity, entity_state::State state)
{
    auto &state_component = entity.get<component::State>();

    if (state_component.state != state)
    {
        state_component.state = state;
        state_changed(entity, state);
    }
}

void entity_funcs::setDirection(const Entity &entity, entity_state::Direction direction)
{
    auto &state_component = entity.get<component::State>();

    if (state_component.direction != direction)
    {
        state_component.direction = direction;
        direction_changed(entity, direction);
    }
}

void entity_funcs::setTarget(const Entity &entity, const Entity &target)
{
    auto &target_component = entity.get<component::Target>();

    if (target_component.target == target)
        return;

    Entity old_target = target_component.target;
    target_component.target = target;

    if (old_target.isValid() && old_target.has<component::Target>())
    {
        auto &old_target_target_component = old_target.get<component::Target>();

        old_target_target_component.lookings.erase(
            std::remove(
                old_target_target_component.lookings.begin(),
                old_target_target_component.lookings.end(),
                entity),
            old_target_target_component.lookings.end());
    }

    if (target.isValid() && target.has<component::Target>())
    {
        auto &target_target_component = target_component.target.get<component::Target>();
        target_target_component.lookings.push_back(entity);
    }

    target_changed(entity, target, old_target);
}

void entity_funcs::setMarker(const Entity &entity, const Entity &marker)
{
    auto &marker_component = entity.get<component::Marker>();

    if (marker_component.marker == marker)
        return;

    if (marker_component.marker.isValid())
        marker_component.marker.destroy();

    marker_component.marker = marker;

    if (marker_component.marker.isValid())
    {
        setParent(marker_component.marker, entity);
        if (!marker_component.marker.isEnabled())
            marker_component.marker.enable();
    }

    marker_changed(entity, marker);
}

void entity_funcs::setDrawableState(const Entity &entity, const std::string &state)
{
    if (entity.has<component::DrawableState>())
    {
        auto &drawable_state_component = entity.get<component::DrawableState>();
        if (drawable_state_component.state)
        {
            drawable_state_component.state->setCurrentState(state);
            drawable_state_changed(entity, state);
        }
    }
    else if (entity.has<component::DrawableAnimation>())
    {
        auto &drawable_animation_component = entity.get<component::DrawableAnimation>();
        if (drawable_animation_component.animation)
        {
            drawable_animation_component.animation->setCurrentState(state);
            drawable_animation_component.animation->start();
            drawable_state_changed(entity, state);
        }
    }
}

void entity_funcs::playSound(const Entity &entity, const std::string &sound_name)
{
    sound_playing(entity, sound_name);
}

void entity_funcs::stopSound(const Entity &entity, const std::string &sound_name)
{
    sound_stopped(entity, sound_name);
}

void entity_funcs::stopAllSound(const Entity &entity)
{
    all_sound_stopped(entity);
}

void entity_funcs::setScript(const Entity &entity, const std::string &script_name)
{
    if (!entity.has<component::Script>())
        return;

    auto &script_component = entity.get<component::Script>();

    script::Script *script = ScriptFactory::createScript(script_name);
    if (!script)
    {
        spdlog::warn("Script not found: {}", script_name);
        return;
    }

    script_component.script.reset(script);
    script_component.script->setEntityToTable(entity);
}

} // namespace fck
