#include "event_handler.h"
#include "event_dispatcher.h"

namespace fck
{

EventHandler::EventHandler(
    const std::vector<int32_t> &event_types,
    const std::function<void(Event *)> &callback,
    const std::string &name)
    : m_event_types{event_types}, m_callback{callback}, m_name{name}, m_enable{true}
{
    EventDispatcher::instance().registerEventHandler(this);
}

EventHandler::~EventHandler()
{
    EventDispatcher::instance().unregisterEventHandler(this);
}

const std::vector<int32_t> &EventHandler::eventTypes() const
{
    return m_event_types;
}

std::string EventHandler::name() const
{
    return m_name;
}

bool EventHandler::isEnable() const
{
    return m_enable;
}

void EventHandler::setEnable(bool enable)
{
    m_enable = enable;
}

void EventHandler::deleteLater()
{
    EventDispatcher::instance().deleteLater(this);
}

void EventHandler::event(Event *event)
{
    if (m_callback)
        m_callback(event);
}

} // namespace fck
