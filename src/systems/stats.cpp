#include "stats.h"

#include "../entity_funcs.h"
#include "../fck/sprite_animation.h"

#include <spdlog/spdlog.h>

namespace fck::system
{

Stats::Stats()
{
}

void Stats::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        auto &stats_component = entity.get<component::Stats>();
        auto &state_component = entity.get<component::State>();

        if (stats_component.damage > 0)
        {
            if (stats_component.armor > 0)
            {
                stats_component.armor -= stats_component.damage;

                if (stats_component.armor < 0)
                    stats_component.armor = 0;

                if (stats_component.armor > stats_component.max_armor)
                    stats_component.armor = stats_component.max_armor;

                entity_funcs::armor_changed(entity, stats_component.armor);

                stats_component.damage = 0;
                continue;
            }

            stats_component.health -= stats_component.damage;

            if (stats_component.health < 0)
                stats_component.health = 0;

            if (stats_component.health > stats_component.max_health)
                stats_component.health = stats_component.max_health;

            entity_funcs::health_changed(entity, stats_component.armor);

            stats_component.damage = 0;
        }

        if (stats_component.health <= 0 && state_component.state != entity_state::DEATH)
        {
            entity_funcs::setState(entity, entity_state::DEATH);
            entity_funcs::setDrawableState(
                entity, entity_state::stateToString(entity_state::DEATH));
        }

        if (state_component.state == entity_state::DEATH)
        {
            stats_component.death_elipsed += delta_time;
            if (stats_component.death_elipsed > stats_component.disappearance_time)
                entity.destroy();
        }
    }
}

} // namespace fck::system
