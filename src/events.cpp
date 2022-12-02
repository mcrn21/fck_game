#include "events.h"

namespace fck
{

KeyboardActionEvent::KeyboardActionEvent(int32_t action, bool finished)
    : Event{event_type::KEYBOARD_ACTION}, m_action{action}, m_finished{finished}
{
}

int32_t KeyboardActionEvent::action() const
{
    return m_action;
}

bool KeyboardActionEvent::isFinished() const
{
    return m_finished;
}

} // namespace fck
