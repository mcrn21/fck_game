#ifndef SPRITEANIMATION_LMXLBFHBSOLK_H
#define SPRITEANIMATION_LMXLBFHBSOLK_H

#include "drawable_animation.h"
#include "sprite.h"

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

#include <string>
#include <unordered_map>

namespace fck
{

class SpriteAnimation : public DrawableAnimation
{
public:
    struct State
    {
        sf::Time interval = sf::milliseconds(0);
        bool repeat;
        sf::IntRect frame_rect;
        sf::Vector2i frame_count;
    };

    SpriteAnimation();
    SpriteAnimation(Sprite *sprite);
    ~SpriteAnimation() = default;

    drawable_animation_type::Type type() const;

    Sprite *sprite() const;
    void setSprite(Sprite *sprite);

    void addState(
        const std::string &state_name,
        const sf::Time &interval,
        bool repeat,
        const sf::IntRect &frame_rect,
        const sf::Vector2i &frame_count);
    void removeState(const std::string &state_name);
    void setCurrentState(const std::string &state_name);
    bool hasStates() const;

    sf::IntRect frameRect() const;

    void start();
    void pause();
    void stop();

    void update(const sf::Time &elapsed);

private:
    Sprite *m_sprite;
    std::unordered_multimap<std::string, State> m_states;
    State *m_current_state;
    int32_t m_current_frame;
    sf::Time m_frame_interval;
    bool m_playing;
    sf::Time m_elapsed;
};

} // namespace fck

#endif // SPRITEANIMATION_LMXLBFHBSOLK_H
