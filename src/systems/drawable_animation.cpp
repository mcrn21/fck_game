#include "drawable_animation.h"

namespace fck::system
{

DrawableAnimation::DrawableAnimation()
{
}

void DrawableAnimation::update(const sf::Time &elapsed)
{
    each<component::DrawableAnimation>(
        [elapsed](Entity &e, component::DrawableAnimation &drawable_animation_component) {
            if (drawable_animation_component.animation)
                drawable_animation_component.animation->update(elapsed);
        });
}

} // namespace fck::system
