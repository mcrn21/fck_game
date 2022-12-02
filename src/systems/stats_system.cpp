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

        if (stats_component.health > 0 && stats_component.damage > 0)
        {
            stats_component.health -= stats_component.damage;
            stats_component.damage = 0;
        }

        if (stats_component.health <= 0 && state_component.state != entity_state::DEATH)
        {
            DrawableAnimationComponent &drawable_animation_component
                = entity.component<DrawableAnimationComponent>();
            SpriteAnimation *sprite_animation = static_cast<SpriteAnimation *>(
                drawable_animation_component.drawable_animation.get());

            EntityUtils::setEntityState(entity, entity_state::DEATH);

            sprite_animation->setCurrentState("death");
            sprite_animation->start();
        }

        if (state_component.state == entity_state::DEATH)
        {
            stats_component.death_elipsed += delta_time;
            if (stats_component.death_elipsed > stats_component.disappearance_time)
                entity.destroy();
        }
    }
}

} // namespace fck
