#include "sprite_state.h"

namespace fck
{

SpriteState::SpriteState() : m_sprite{nullptr}
{
}

SpriteState::SpriteState(sf::Sprite &sprite) : m_sprite{&sprite}
{
}

sf::Sprite *SpriteState::getSprite() const
{
    return m_sprite;
}

void SpriteState::setSprite(sf::Sprite &sprite)
{
    m_sprite = &sprite;

    m_states.clear();
    addState("default", m_sprite->getTextureRect());
}

std::string SpriteState::getCurrentState() const
{
    return m_current_state;
}

void SpriteState::setCurrentState(const std::string &state_name)
{
    if (!m_sprite)
        return;

    auto states_found = m_states.find(state_name);
    if (states_found != m_states.end())
    {
        m_current_state = state_name;
        m_sprite->setTextureRect(states_found->second);
    }
}

std::vector<std::string> SpriteState::getStates() const
{
    std::vector<std::string> states;

    for (const auto &it : m_states)
        states.push_back(it.first);

    return states;
}

void SpriteState::addState(const std::string &state_name, const sf::IntRect &state_rect)
{
    m_states.emplace(state_name, state_rect);
}

void SpriteState::removeState(const std::string &state_name)
{
    m_states.erase(state_name);
}

bool SpriteState::hasState(const std::string &state_name) const
{
    return m_states.find(state_name) != m_states.end();
}

sf::IntRect SpriteState::getTextureRect(const std::string &state_name) const
{
    return m_states.at(state_name);
}

} // namespace fck
