#include "timer.h"
#include "event_dispatcher.h"

namespace fck
{

Timer::Timer(sf::Time interval, bool repeat, const std::function<void()> &callback)
    : m_interval{interval},
      m_repeat{repeat},
      m_callback{callback},
      m_singleshot{false},
      m_enable{false}
{
    EventDispatcher::instance().registerTimer(this);
}

Timer::~Timer()
{
    EventDispatcher::instance().unregisterTimer(this);
}

sf::Time Timer::interval() const
{
    return m_interval;
}

void Timer::setInterval(sf::Time interval)
{
    m_interval = interval;
}

bool Timer::isRepeat() const
{
    return m_repeat;
}

void Timer::setRepeat(bool repeat)
{
    m_repeat = repeat;
}

bool Timer::isEnable() const
{
    return m_enable;
}

void Timer::start()
{
    m_start_time = EventDispatcher::instance().calculateTimerStartTime(this);
    m_enable = true;
}

void Timer::stop()
{
    m_enable = false;
}

void Timer::singleShot(sf::Time interval, const std::function<void()> &callback)
{
    Timer *timer = new Timer{interval, false, callback};
    timer->m_singleshot = true;
    timer->start();
}

} // namespace fck
