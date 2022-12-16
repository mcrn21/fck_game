#ifndef SPRITESTATE_NRQBLMFRMEHP_H
#define SPRITESTATE_NRQBLMFRMEHP_H

#include "drawable_state.h"

#include <SFML/Graphics.hpp>

#include <unordered_map>

namespace fck
{

class SpriteState : public DrawableState
{
public:
    SpriteState();
    SpriteState(sf::Sprite &sprite);
    ~SpriteState() = default;

    sf::Sprite *getSprite() const;
    void setSprite(sf::Sprite &sprite);

    std::string getCurrentState() const;
    void setCurrentState(const std::string &state_name);

    std::vector<std::string> getStates() const;

    void addState(const std::string &state_name, const sf::IntRect &state_rect);
    void removeState(const std::string &state_name);
    bool hasState(const std::string &state_name) const;

    sf::IntRect getTextureRect(const std::string &state_name) const;

private:
    sf::Sprite *m_sprite;
    std::string m_current_state;
    std::unordered_map<std::string, sf::IntRect> m_states;
};

} // namespace fck

#endif // SPRITESTATE_NRQBLMFRMEHP_H
