#include "player_script.h"

#include "../entity_utils.h"
#include "../fck_game.h"
#include "../knowledge_base.h"

#include "../components/components.h"

#include <spdlog/spdlog.h>

namespace fck
{

PlayerScript::PlayerScript()
{
}

void PlayerScript::update(const Entity &entity, double delta_time)
{
    // Update targets
    updateTarget(entity);
}

void PlayerScript::onEntityCollided(const Entity &entity, const Entity &other)
{
}

void PlayerScript::updateTarget(const Entity &entity)
{
    PlayerComponent &player_component = entity.component<PlayerComponent>();
    TargetComponent &target_component = entity.component<TargetComponent>();
    LookAroundComponent &look_around_component = entity.component<LookAroundComponent>();

    if (!target_component.target.isValid())
    {
        for (const Entity &looked_entity : look_around_component.look_at_entities)
        {
            StateComponent &looked_entity_state_component
                = looked_entity.component<StateComponent>();
            if (looked_entity_state_component.state != entity_state::DEATH)
            {
                spdlog::debug("Set target {}", looked_entity.id().index());
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
                StateComponent &looked_entity_state_component
                    = looked_entity.component<StateComponent>();
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
                StateComponent &looked_entity_state_component
                    = looked_entity.component<StateComponent>();

                if (looked_entity_state_component.state != entity_state::DEATH)
                {
                    spdlog::debug("Change target {}", looked_entity.id().index());
                    entity::set_target.emit(entity, looked_entity);
                    break;
                }
            }
        }
    }
}

} // namespace fck
