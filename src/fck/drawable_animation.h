#ifndef DRAWABLEANIMATION_VGVIBFQLTYYQ_H
#define DRAWABLEANIMATION_VGVIBFQLTYYQ_H

#include "common.h"

#include "SFML/System/Time.hpp"

namespace fck
{

class DrawableAnimation
{
public:
    DrawableAnimation();
    virtual ~DrawableAnimation() = default;

    virtual void setCurrentState([[maybe_unused]] const std::string &state_name);

    virtual void start();
    virtual void restart();
    virtual void pause();
    virtual void stop();

    virtual void update([[maybe_unused]] const sf::Time &elapsed);
};

} // namespace fck

#endif // DRAWABLEANIMATION_VGVIBFQLTYYQ_H
