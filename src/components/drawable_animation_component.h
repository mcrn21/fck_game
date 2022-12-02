#ifndef DRAWABLEANIMATIONCOMPONENT_H
#define DRAWABLEANIMATIONCOMPONENT_H

#include "../fck/drawable_animation.h"

#include <memory>

namespace fck
{

struct DrawableAnimationComponent
{
    std::unique_ptr<DrawableAnimation> drawable_animation;
};

} // namespace fck

#endif // DRAWABLEANIMATIONCOMPONENT_H
