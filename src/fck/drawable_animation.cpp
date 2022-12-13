#include "drawable_animation.h"

namespace fck
{

DrawableAnimation::DrawableAnimation()
{
}

void DrawableAnimation::setCurrentState(const std::string &state_name)
{
}

void DrawableAnimation::start()
{
}

void DrawableAnimation::restart()
{
    stop();
    start();
}

void DrawableAnimation::pause()
{
}

void DrawableAnimation::stop()
{
}

void DrawableAnimation::update(const sf::Time &elapsed)
{
}

} // namespace fck
