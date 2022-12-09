#ifndef DRAWABLEANIMATION_BTAKMZNXTGBV_H
#define DRAWABLEANIMATION_BTAKMZNXTGBV_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck::system
{

class DrawableAnimation : public System<component::DrawableAnimation>
{
public:
    DrawableAnimation();
    ~DrawableAnimation() = default;

    void update(const sf::Time &elapsed);
};

} // namespace fck::system

#endif // DRAWABLEANIMATION_BTAKMZNXTGBV_H
