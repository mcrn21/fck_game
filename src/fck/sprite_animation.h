#ifndef SPRITEANIMATION_LMXLBFHBSOLK_H
#define SPRITEANIMATION_LMXLBFHBSOLK_H

#include "drawable_animation.h"

#include <SFML/Graphics.hpp>

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
        sf::IntRect texture_rect;
        sf::Vector2i frame_count;
    };

    SpriteAnimation();
    SpriteAnimation(sf::Sprite &sprite);
    ~SpriteAnimation() = default;

    sf::Sprite *getSprite() const;
    void setSprite(sf::Sprite &sprite);

    void setCurrentState(const std::string &state_name);
    std::vector<std::string> getStates() const;

    void addState(
        const std::string &state_name,
        const sf::Time &interval,
        bool repeat,
        const sf::IntRect &frame_rect,
        const sf::Vector2i &frame_count);
    void removeState(const std::string &state_name);
    bool hasStates() const;

    sf::IntRect getTextureRect() const;

    void start();
    void pause();
    void stop();

    void update(const sf::Time &elapsed);

private:
    sf::Sprite *m_sprite;
    std::unordered_multimap<std::string, State> m_states;
    State *m_current_state;
    int32_t m_current_frame;
    sf::Time m_frame_interval;
    bool m_playing;
    sf::Time m_elapsed;
};

} // namespace fck

#endif // SPRITEANIMATION_LMXLBFHBSOLK_H
