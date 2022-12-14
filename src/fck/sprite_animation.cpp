#include "sprite_animation.h"

namespace fck
{

SpriteAnimation::SpriteAnimation()
    : m_sprite{nullptr}, m_current_state{nullptr}, m_current_frame{0}, m_playing{false}
{
}

SpriteAnimation::SpriteAnimation(Sprite &sprite)
    : m_sprite{&sprite}, m_current_state{nullptr}, m_current_frame{0}, m_playing{false}
{
}

Sprite *SpriteAnimation::getSprite() const
{
    return m_sprite;
}

void SpriteAnimation::setSprite(Sprite &sprite)
{
    m_sprite = &sprite;
    stop();
}

void SpriteAnimation::setCurrentState(const std::string &state_name)
{
    auto state_found = m_states.find(state_name);
    if (state_found == m_states.end())
        return;

    if (m_current_state == &state_found->second)
        return;

    stop();

    m_current_state = &state_found->second;
    m_current_frame = 0;
    m_frame_interval = sf::milliseconds(
        state_found->second.interval.asMilliseconds()
        / (state_found->second.frame_count.x * state_found->second.frame_count.y));

    m_sprite->setTextureRect(getTextureRect());

    m_current_frame = 1;
}

std::vector<std::string> SpriteAnimation::getStates() const
{
    std::vector<std::string> states;

    for (const auto &it : m_states)
        states.push_back(it.first);

    return states;
}

void SpriteAnimation::addState(
    const std::string &state_name,
    const sf::Time &interval,
    bool repeat,
    const sf::IntRect &frame_rect,
    const sf::Vector2i &frame_count)
{
    if (state_name.empty())
        return;

    auto state_found = m_states.find(state_name);
    if (state_found != m_states.end())
        m_states.erase(state_found);

    m_states.emplace(state_name, State{interval, repeat, frame_rect, frame_count});
    if (m_states.size() == 1)
        setCurrentState(state_name);
}

void SpriteAnimation::removeState(const std::string &state_name)
{
    auto state_found = m_states.find(state_name);
    if (state_found != m_states.end())
    {
        if (m_states.size() == 1)
        {
            stop();
            m_current_state = nullptr;
        }
        else
        {
            if ((&state_found->second) != m_current_state)
                setCurrentState(m_states.begin()->first);
        }

        m_states.erase(state_found);
    }
}

bool SpriteAnimation::hasStates() const
{
    return !m_states.empty();
}

sf::IntRect SpriteAnimation::getTextureRect() const
{
    if (m_current_state)
    {
        int32_t current_frame_x = m_current_frame % m_current_state->frame_count.x;
        int32_t current_frame_y = m_current_frame / m_current_state->frame_count.x;

        return sf::IntRect{
            sf::Vector2i(
                m_current_state->texture_rect.left
                    + current_frame_x * m_current_state->texture_rect.width,
                m_current_state->texture_rect.top
                    + current_frame_y * m_current_state->texture_rect.height),
            sf::Vector2i(
                m_current_state->texture_rect.width, m_current_state->texture_rect.height)};
    }
    return sf::IntRect{};
}

void SpriteAnimation::start()
{
    m_playing = true;
}

void SpriteAnimation::pause()
{
    m_playing = false;
}

void SpriteAnimation::stop()
{
    m_playing = false;
    m_current_frame = 0;
}

void SpriteAnimation::update(const sf::Time &elapsed)
{
    if (m_playing && m_sprite && m_current_state)
    {
        m_elapsed += elapsed;
        if (m_elapsed > m_frame_interval)
        {
            m_elapsed = sf::Time::Zero;
            m_sprite->setTextureRect(getTextureRect());
            ++m_current_frame;
            if (m_current_frame == m_current_state->frame_count.x * m_current_state->frame_count.y)
            {
                if (m_current_state->repeat)
                    m_current_frame = 0;
                else
                    m_playing = false;
            }
        }
    }
}

} // namespace fck
