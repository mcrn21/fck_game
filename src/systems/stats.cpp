#include "stats.h"

#include "../entity_utils.h"

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
        component::Stats &stats_component = entity.getComponent<component::Stats>();
        component::State &state_component = entity.getComponent<component::State>();

        if (stats_component.damage > 0)
        {
            if (stats_component.armor > 0)
            {
                entity::set_armor.emit(entity, stats_component.armor - stats_component.damage);
                stats_component.damage = 0;
                continue;
            }

            entity::set_heath.emit(entity, stats_component.health - stats_component.damage);
            stats_component.damage = 0;
        }

        if (stats_component.health <= 0 && state_component.state != entity_state::DEATH)
            entity::set_state.emit(entity, entity_state::DEATH);

        if (state_component.state == entity_state::DEATH)
        {
            stats_component.death_elipsed += delta_time;
            if (stats_component.death_elipsed > stats_component.disappearance_time)
                entity::destroy.emit(entity);
        }
    }
}

} // namespace fck::system
