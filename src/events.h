#ifndef EVENTS_H
#define EVENTS_H

#include "fck/event.h"
#include "fck_common.h"

namespace fck
{

class KeyboardActionEvent : public Event
{
public:
    KeyboardActionEvent(int32_t action, bool finished = false);
    ~KeyboardActionEvent() = default;

    int32_t action() const;
    bool isFinished() const;

private:
    int32_t m_action;
    bool m_finished;
};

} // namespace fck

#endif // EVENTS_H
