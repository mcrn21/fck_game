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

void PlayerScript::updateTarget(const Entity &entity)
{
    PlayerComponent &player_component = entity.component<PlayerComponent>();
    TargetComponent &target_component = entity.component<TargetComponent>();
    LookAroundComponent &look_around_component = entity.component<LookAroundComponent>();

    if (!target_component.target.isValid())
    {
        if (target_component.target_mark.isValid())
            target_component.target_mark.destroy();

        if (!look_around_component.look_at_entities.empty())
        {
            for (const Entity &looked_entity : look_around_component.look_at_entities)
            {
                StateComponent &looked_entity_state_component
                    = looked_entity.component<StateComponent>();

                if (looked_entity_state_component.state != entity_state::DEATH)
                {
                    EntityUtils::setEntityTarget(
                        entity,
                        looked_entity,
                        KnowledgeBase::createEntity("target", entity.world()));
                    break;
                }
            }
        }
    }
    else
    {
        StateComponent &target_state_component
            = target_component.target.component<StateComponent>();
        if (target_state_component.state == entity_state::DEATH)
        {
            if (target_component.target_mark.isValid())
                target_component.target_mark.destroy();
        }

        if (look_around_component.look_at_entities.empty())
        {
            EntityUtils::setEntityTarget(entity, Entity{});
        }
        else if (player_component.need_change_target)
        {
            auto looked_entities_found = std::find(
                look_around_component.look_at_entities.begin(),
                look_around_component.look_at_entities.end(),
                target_component.target);
            if (looked_entities_found != look_around_component.look_at_entities.end())
            {
                ++looked_entities_found;
                if (looked_entities_found == look_around_component.look_at_entities.end())
                    looked_entities_found = look_around_component.look_at_entities.begin();

                Entity &looked_entity = *looked_entities_found;
                StateComponent &looked_entity_state_component
                    = looked_entity.component<StateComponent>();

                if (looked_entity_state_component.state != entity_state::DEATH)
                {
                    EntityUtils::setEntityTarget(
                        entity,
                        looked_entity,
                        KnowledgeBase::createEntity("target", entity.world()));
                }
            }

            player_component.need_change_target = false;
        }
    }
}

} // namespace fck
