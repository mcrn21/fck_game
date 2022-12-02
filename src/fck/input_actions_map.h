#ifndef INPUTACTIONS_H
#define INPUTACTIONS_H

#include "event_handler.h"
#include "input_action.h"
#include "sigslot.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace fck
{

template<typename T>
class InputActionsMap : public EventHandler
{
public:
    InputActionsMap(const std::string &name = std::string{});
    ~InputActionsMap() = default;

    InputAction &operator[](const T &id);
    void setActions(const std::unordered_map<T, InputAction> &input_actions);
    void removeAction(const T &id);
    void clearActions();

    bool isActive(const T &id) const;

    void event(Event *event);

public:
    Signal<T> action_activated;

private:
    std::unordered_map<T, InputAction> m_input_actions;
};

template<typename T>
InputActionsMap<T>::InputActionsMap(const std::string &name)
    : EventHandler{{sf::Event::KeyPressed, sf::Event::KeyReleased}, {}, name}
{
}

template<typename T>
InputAction &InputActionsMap<T>::operator[](const T &id)
{
    return m_input_actions[id];
}

template<typename T>
void InputActionsMap<T>::setActions(const std::unordered_map<T, InputAction> &input_actions)
{
    m_input_actions = input_actions;
}

template<typename T>
void InputActionsMap<T>::removeAction(const T &id)
{
    m_input_actions.erase(id);
}

template<typename T>
void InputActionsMap<T>::clearActions()
{
    m_input_actions.clear();
}

template<typename T>
bool InputActionsMap<T>::isActive(const T &id) const
{
    auto input_actions_found = m_input_actions.find(id);
    if (input_actions_found == m_input_actions.end())
        return false;
    return input_actions_found->second.activated;
}

template<typename T>
void InputActionsMap<T>::event(Event *event)
{
    SfmlEvent *sfml_event = static_cast<SfmlEvent *>(event);

    uint64_t mask = 0;
    mask ^= sfml_event->get().key.code << 1;

    for (auto &it : m_input_actions)
    {
        if (it.second.mask == mask)
        {
            if (sfml_event->get().type == sf::Event::KeyPressed)
            {
                if (it.second.type == InputAction::PRESS_ONCE && !it.second.caused
                    && !it.second.activated)
                {
                    it.second.activated = true;
                    it.second.caused = true;
                    action_activated.emit(it.first);
                    continue;
                }

                if (it.second.type == InputAction::PRESS_ONCE && it.second.caused
                    && it.second.activated)
                {
                    it.second.activated = false;
                    continue;
                }

                if (it.second.type == InputAction::HOLD && !it.second.caused)
                {
                    it.second.activated = true;
                    it.second.caused = true;
                    action_activated.emit(it.first);
                    continue;
                }
            }

            if (sfml_event->get().type == sf::Event::KeyReleased)
            {
                if (it.second.type == InputAction::PRESS_ONCE
                    || it.second.type == InputAction::HOLD)
                {
                    it.second.caused = false;
                    it.second.activated = false;
                }

                if (it.second.type == InputAction::RELEASE_ONCE)
                {
                    it.second.activated = true;
                    action_activated.emit(it.first);
                    it.second.activated = false;
                    continue;
                }
            }
        }
    }
}

} // namespace fck

#endif // INPUTACTIONS_H
