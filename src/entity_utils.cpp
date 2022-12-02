#include "entity_utils.h"

namespace fck
{

void EntityUtils::moveEntity(const Entity &entity, const sf::Vector2f &offset)
{
    TransformComponent &transform_component = entity.component<TransformComponent>();
    transform_component.transform.move(offset);
    entity_moved.emit(entity, offset);
}

void EntityUtils::setEntityPosition(const Entity &entity, const sf::Vector2f &position)
{
    TransformComponent &transform_component = entity.component<TransformComponent>();
    sf::Vector2f offset = position - transform_component.transform.getPosition();
    transform_component.transform.setPosition(position);
    entity_moved.emit(entity, offset);
}

void EntityUtils::setEntityParent(const Entity &entity, const Entity &parent)
{
    TransformComponent &transform_component = entity.component<TransformComponent>();
    auto old_parent = transform_component.parent;

    if (old_parent == parent)
        return;

    if (old_parent.isValid())
    {
        TransformComponent &transform_component = old_parent.component<TransformComponent>();
        transform_component.children.erase(
            std::remove(
                transform_component.children.begin(), transform_component.children.end(), entity),
            transform_component.children.end());
    }

    transform_component.parent = parent;

    if (parent.isValid())
    {
        TransformComponent &transform_component = parent.component<TransformComponent>();
        transform_component.children.push_back(entity);
    }

    entity_parent_changed.emit(entity, old_parent);
}

std::list<Entity> EntityUtils::lookAround(const Entity &entity, const sf::FloatRect &around_bounds)
{
    SceneComponent &scene_component = entity.component<SceneComponent>();

    if (!scene_component.tree)
        return {};

    std::list<Entity> founded_entities;
    scene_component.tree->querry(around_bounds, [&](int32_t id) {
        Entity other = scene_component.tree->userData(id);
        if (other != entity)
            founded_entities.push_back(other);
        return true;
    });
    return founded_entities;
}

void EntityUtils::setEntityTarget(
    const Entity &entity, const Entity &target, const Entity &target_mark)
{
    TargetComponent &target_component = entity.component<TargetComponent>();

    if (target_component.target.isValid())
    {
        target_component.target = {};
        if (target_component.target_mark.isValid())
        {
            setEntityParent(target_component.target_mark, Entity{});
            target_component.target_mark.destroy();
        }
    }

    target_component.target = target;

    if (target_component.target.isValid())
    {
        if (target_mark.isValid())
        {
            target_component.target_mark = target_mark;
            if (!target_component.target_mark.isEnabled())
                target_component.target_mark.enable();

            target_component.target_mark.component<TransformComponent>().transform.setPosition(
                target_component.target.component<TransformComponent>().transform.getPosition());

            setEntityParent(target_component.target_mark, target_component.target);
            EntityUtils::entity_moved.emit(target_component.target_mark, sf::Vector2f{});
        }
    }
}

void EntityUtils::setEntityState(const Entity &entity, entity_state::State state)
{
    StateComponent &state_component = entity.component<StateComponent>();

    if (state == state_component.state)
        return;

    entity_state::State old_state = state_component.state;
    state_component.state = state;
    entity_state_changed.emit(entity, old_state);
}

void EntityUtils::setEntityDirection(const Entity &entity, entity_state::Direction direction)
{
    StateComponent &state_component = entity.component<StateComponent>();

    if (direction == state_component.direction)
        return;

    entity_state::Direction old_direction = state_component.direction;
    state_component.direction = direction;
    entity_direction_changed.emit(entity, old_direction);
}

// transform
Signal<const Entity &, const sf::Vector2f &> EntityUtils::entity_moved;
Signal<const Entity &, const Entity &> EntityUtils::entity_parent_changed;

// state
Signal<const Entity &, entity_state::State> EntityUtils::entity_state_changed;
Signal<const Entity &, entity_state::Direction> EntityUtils::entity_direction_changed;

} // namespace fck
