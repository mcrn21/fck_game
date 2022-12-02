#include "drawable_animation.h"

namespace fck
{

DrawableAnimation::DrawableAnimation()
{
}

drawable_animation_type::Type DrawableAnimation::type() const
{
    return drawable_animation_type::NO_TYPE;
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
