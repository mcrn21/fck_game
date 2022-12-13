#include "event_dispatcher.h"

#include <spdlog/spdlog.h>

namespace fck
{

void EventDispatcher::push(int32_t type, EventHandler *receiver)
{
    instance().m_need_add_event_details.push({std::make_unique<Event>(type), receiver});
}

void EventDispatcher::push(int32_t type, const std::string &receiver_name)
{
    push(type, findHandlerByName(receiver_name));
}

void EventDispatcher::push(Event *event, EventHandler *receiver)
{
    instance().m_need_add_event_details.push({std::unique_ptr<Event>(event), receiver});
}

void EventDispatcher::push(Event *event, const std::string &receiver_name)
{
    push(event, findHandlerByName(receiver_name));
}

EventHandler *EventDispatcher::findHandlerByName(const std::string &name)
{
    auto found = instance().m_event_handlers_names.find(name);
    if (found == instance().m_event_handlers_names.end())
        return nullptr;
    return found->second;
}

void EventDispatcher::deleteLater(EventHandler *event_handler)
{
    runTask([event_handler]() { delete event_handler; });
}

void EventDispatcher::runTask(const std::function<void()> &task)
{
    push(new TaskEvent{task});
}

void EventDispatcher::runTasks(const std::vector<std::function<bool()>> &tasks)
{
    push(new TasksEvent{tasks, 0});
}

void EventDispatcher::update(sf::Time elapsed)
{
    instance()._update(elapsed);
}

EventDispatcher &EventDispatcher::instance()
{
    static EventDispatcher event_dispatcher;
    return event_dispatcher;
}

void EventDispatcher::_update(sf::Time elapsed)
{
    m_elapsed_time += elapsed;

    for (Timer *timer : m_timers)
    {
        if (timer->m_enable && m_elapsed_time >= timer->m_start_time)
        {
            timer->m_callback();
            timer->m_enable = false;

            if (timer->m_singleshot)
            {
                runTask([timer]() { delete timer; });
                continue;
            }

            if (timer->m_repeat)
                timer->start();
        }
    }

    if (m_need_add_event_details.empty())
        return;

    std::swap(m_event_details, m_need_add_event_details);

    // Clear need queue
    static std::queue<EventDetail> empty_event_details;
    std::swap(m_need_add_event_details, empty_event_details);

    while (!m_event_details.empty())
    {
        EventDetail &event_detail = m_event_details.front();

        if (event_detail.event->type() == event_type::TASK)
        {
            TaskEvent *task_event = static_cast<TaskEvent *>(event_detail.event.get());
            task_event->call();
        }
        else if (event_detail.event->type() == event_type::TASKS)
        {
            TasksEvent *task_event = static_cast<TasksEvent *>(event_detail.event.release());
            if (task_event->call())
                push(task_event);
            else
                delete task_event;
        }
        else
        {
            if (event_detail.receiver)
            {
                if (event_detail.receiver->isEnable())
                    event_detail.receiver->event(event_detail.event.get());
            }
            else
            {
                auto range = m_event_handlers_event_types.equal_range(event_detail.event->type());
                for (auto it = range.first; it != range.second; ++it)
                {
                    if (event_detail.event->isAccepted())
                        break;
                    if (it->second->isEnable())
                        it->second->event(event_detail.event.get());
                }
            }
        }

        m_event_details.pop();
    }
}

void EventDispatcher::registerEventHandler(EventHandler *event_handler)
{
    if (!event_handler)
        return;

    if (!event_handler->getName().empty())
        m_event_handlers_names.emplace(event_handler->getName(), event_handler);

    for (int32_t event_type : event_handler->getEventTypes())
        m_event_handlers_event_types.emplace(event_type, event_handler);
}

void EventDispatcher::unregisterEventHandler(EventHandler *event_handler)
{
    if (!event_handler)
        return;

    if (!event_handler->getName().empty())
    {
        auto name_found = m_event_handlers_names.find(event_handler->getName());
        if (name_found != m_event_handlers_names.end())
            m_event_handlers_names.erase(name_found);
    }

    for (int32_t event_type : event_handler->getEventTypes())
    {
        auto range = m_event_handlers_event_types.equal_range(event_type);
        for (auto it = range.first; it != range.second; ++it)
        {
            if (it->second == event_handler)
            {
                m_event_handlers_event_types.erase(it);
                break;
            }
        }
    }
}

void EventDispatcher::registerTimer(Timer *timer)
{
    if (!timer)
        return;
    m_timers.push_back(timer);
}

void EventDispatcher::unregisterTimer(Timer *timer)
{
    if (!timer)
        return;
    m_timers.erase(std::remove(m_timers.begin(), m_timers.end(), timer), m_timers.end());
}

sf::Time EventDispatcher::calculateTimerStartTime(Timer *timer)
{
    return sf::milliseconds(m_elapsed_time.asMilliseconds() + timer->getInterval().asMilliseconds());
}

} // namespace fck
