#ifndef INPUTACTION_H
#define INPUTACTION_H

#include <SFML/Window/Keyboard.hpp>

#include <cstdint>

namespace fck
{

struct InputAction
{

public:
    enum ActionType
    {
        HOLD,
        PRESS_ONCE,
        RELEASE_ONCE
    };

    InputAction() : mask{0}, type{HOLD}, activated{false}, caused{false}
    {
    }

    InputAction(sf::Keyboard::Key code, ActionType type = HOLD)
        : type{type}, activated{false}, caused{false}
    {
        mask = 0;
        mask ^= code << 1;
    }
    ~InputAction() = default;

    ActionType type;
    uint64_t mask;
    bool activated;
    bool caused;
};

} // namespace fck

#endif // INPUTACTION_H
