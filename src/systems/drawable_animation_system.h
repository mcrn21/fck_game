#ifndef DRAWABLEANIMATIONSYSTEM_H
#define DRAWABLEANIMATIONSYSTEM_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck
{

class DrawableAnimationSystem : public System<DrawableAnimationComponent>
{
public:
    DrawableAnimationSystem();
    ~DrawableAnimationSystem() = default;

    void update(const sf::Time &elapsed);
};

} // namespace fck

#endif // DRAWABLEANIMATIONSYSTEM_H
