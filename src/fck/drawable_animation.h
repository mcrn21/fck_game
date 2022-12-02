#ifndef DRAWABLEANIMATION_H
#define DRAWABLEANIMATION_H

#include "common.h"

#include "SFML/System/Time.hpp"

namespace fck
{

class DrawableAnimation
{
public:
    DrawableAnimation();
    virtual ~DrawableAnimation() = default;

    virtual drawable_animation_type::Type type() const;

    virtual void setCurrentState([[maybe_unused]] const std::string &state_name);

    virtual void start();
    virtual void restart();
    virtual void pause();
    virtual void stop();

    virtual void update([[maybe_unused]] const sf::Time &elapsed);
};

} // namespace fck

#endif // DRAWABLEANIMATION_H
