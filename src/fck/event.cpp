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

TasksEvent::TasksEvent(const std::vector<std::function<bool()>> &callbacks, int32_t index)
    : Event{event_type::TASKS}, m_callbacks{callbacks}, m_index{index}
{
}

bool TasksEvent::call()
{
    if (m_callbacks.empty())
        return false;

    if (!m_callbacks[m_index]())
        return false;

    ++m_index;
    if (m_index >= m_callbacks.size())
        return false;

    return true;
}

} // namespace fck
