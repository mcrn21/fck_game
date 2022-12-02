#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "event.h"
#include "event_handler.h"
#include "task_sequence.h"
#include "timer.h"

#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include <SFML/System/Time.hpp>

namespace fck
{

class EventDispatcher
{
    friend class EventHandler;
    friend class Timer;

public:
    static void push(int32_t type, EventHandler *receiver = nullptr);
    static void push(int32_t type, const std::string &receiver_name);
    static void push(Event *event, EventHandler *receiver = nullptr);
    static void push(Event *event, const std::string &receiver_name);

    static EventHandler *findHandlerByName(const std::string &name);
    static void deleteLater(EventHandler *event_handler);

    static void runTask(const std::function<void()> &task);

    static void update(sf::Time elapsed);

private:
    static EventDispatcher &instance();

    EventDispatcher() = default;
    ~EventDispatcher() = default;

    void _update(sf::Time elapsed);

    void registerEventHandler(EventHandler *event_handler);
    void unregisterEventHandler(EventHandler *event_handler);

    void registerTimer(Timer *timer);
    void unregisterTimer(Timer *timer);
    sf::Time calculateTimerStartTime(Timer *timer);

private:
    struct EventDetail
    {
        std::unique_ptr<Event> event;
        EventHandler *receiver = nullptr;
    };

    std::queue<EventDetail> m_need_add_event_details;
    std::queue<EventDetail> m_event_details;

    std::unordered_map<std::string, EventHandler *> m_event_handlers_names;
    std::unordered_multimap<int32_t, EventHandler *> m_event_handlers_event_types;

    sf::Time m_elapsed_time;

    std::vector<Timer *> m_timers;
};

} // namespace fck

#endif // EVENTDISPATCHER_H
