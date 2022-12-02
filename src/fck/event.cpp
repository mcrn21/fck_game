#include "event.h"

#include <spdlog/spdlog.h>

namespace fck
{

Event::Event(int32_t type) : m_type{type}, m_accepted{false}
{
}

int32_t Event::type() const
{
    return m_type;
}

bool Event::isAccepted() const
{
    return m_accepted;
}

void Event::accept()
{
    m_accepted = true;
}

SfmlEvent::SfmlEvent(const sf::Event &event) : Event{event.type}, m_event{event}
{
}

const sf::Event &SfmlEvent::get() const
{
    return m_event;
}

TaskEvent::TaskEvent(const std::function<void()> &callback)
    : Event{event_type::TASK}, m_callback{callback}
{
}

void TaskEvent::call()
{
    if (m_callback)
        m_callback();
}

} // namespace fck
