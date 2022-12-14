#include "drawable_animation.h"

namespace fck
{

DrawableAnimation::DrawableAnimation()
{
}

void DrawableAnimation::setCurrentState(const std::string &state_name)
{
}

std::vector<std::string> DrawableAnimation::getStates() const
{
    return {};
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
