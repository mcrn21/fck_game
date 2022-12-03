#include "stats_system.h"

#include "../entity_utils.h"

#include "../fck/sprite_animation.h"

#include <spdlog/spdlog.h>

namespace fck
{

StatsSystem::StatsSystem()
{
}

void StatsSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        StatsComponent &stats_component = entity.component<StatsComponent>();
        StateComponent &state_component = entity.component<StateComponent>();

        if (stats_component.damage > 0)
        {
            if (stats_component.armor > 0)
            {
                EntityUtils::addEntityArmor(entity, -stats_component.damage);
                stats_component.damage = 0;
                continue;
            }

            EntityUtils::addEntityHealth(entity, -stats_component.damage);
            stats_component.damage = 0;
        }

        if (stats_component.health <= 0 && state_component.state != entity_state::DEATH)
            EntityUtils::setEntityState(entity, entity_state::DEATH);

        if (state_component.state == entity_state::DEATH)
        {
            stats_component.death_elipsed += delta_time;
            if (stats_component.death_elipsed > stats_component.disappearance_time)
                entity.destroy();
        }
    }
}

} // namespace fck
