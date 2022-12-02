#include "drawable_animation_system.h"

namespace fck
{

DrawableAnimationSystem::DrawableAnimationSystem()
{
}

void DrawableAnimationSystem::update(const sf::Time &elapsed)
{
    each<DrawableAnimationComponent>(
        [elapsed](Entity &e, DrawableAnimationComponent &drawable_animation_component) {
            if (drawable_animation_component.drawable_animation)
                drawable_animation_component.drawable_animation->update(elapsed);
        });
}

} // namespace fck
