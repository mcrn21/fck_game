#include "player.h"

#include "../entity_utils.h"
#include "../fck_game.h"
#include "../knowledge_base.h"

#include "../components/components.h"

#include <spdlog/spdlog.h>

namespace fck::entity_script
{

Player::Player()
{
}

void Player::update(const Entity &entity, double delta_time)
{
    // Update targets
    updateTarget(entity);
}

void Player::onEntityCollided(const Entity &entity, const Entity &other)
{
}

void Player::updateTarget(const Entity &entity)
{
    component::Player &player_component = entity.getComponent<component::Player>();
    component::Target &target_component = entity.getComponent<component::Target>();
    component::LookAround &look_around_component = entity.getComponent<component::LookAround>();

    if (!target_component.target.isValid())
    {
        for (const Entity &looked_entity : look_around_component.look_at_entities)
        {
            component::State &looked_entity_state_component
                = looked_entity.getComponent<component::State>();
            if (looked_entity_state_component.state != entity_state::DEATH)
            {
                spdlog::debug("Set target {}", looked_entity.getId().getIndex());
                entity::set_target.emit(entity, looked_entity);
                break;
            }
        }
    }
    else
    {
        auto look_at_entities_found = std::find(
            look_around_component.look_at_entities.begin(),
            look_around_component.look_at_entities.end(),
            target_component.target);

        if (look_at_entities_found == look_around_component.look_at_entities.end())
        {
            entity::set_target.emit(entity, Entity{});
            for (const Entity &looked_entity : look_around_component.look_at_entities)
            {
                component::State &looked_entity_state_component
                    = looked_entity.getComponent<component::State>();
                if (looked_entity_state_component.state != entity_state::DEATH)
                {
                    entity::set_target.emit(entity, looked_entity);
                    break;
                }
            }
        }
        else if (player_component.need_change_target)
        {
            player_component.need_change_target = false;

            if (look_around_component.look_at_entities.size() == 1)
                return;

            auto current_target_it = look_at_entities_found;
            while (true)
            {
                ++look_at_entities_found;
                if (look_at_entities_found == look_around_component.look_at_entities.end())
                    look_at_entities_found = look_around_component.look_at_entities.begin();

                if (look_at_entities_found == current_target_it)
                {
                    entity::set_target.emit(entity, Entity{});
                    break;
                }

                Entity &looked_entity = *look_at_entities_found;
                component::State &looked_entity_state_component
                    = looked_entity.getComponent<component::State>();

                if (looked_entity_state_component.state != entity_state::DEATH)
                {
                    spdlog::debug("Change target {}", looked_entity.getId().getIndex());
                    entity::set_target.emit(entity, looked_entity);
                    break;
                }
            }
        }
    }
}

} // namespace fck::entity_script
