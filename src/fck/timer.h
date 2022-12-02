#ifndef TIMER_H
#define TIMER_H

#include <SFML/System/Time.hpp>

#include <functional>

namespace fck
{

class Timer
{
    friend class EventDispatcher;

public:
    Timer(sf::Time interval, bool repeat, const std::function<void()> &callback);
    ~Timer();

    sf::Time interval() const;
    void setInterval(sf::Time interval);

    bool isRepeat() const;
    void setRepeat(bool repeat);

    bool isEnable() const;

    void start();
    void stop();

    static void singleShot(sf::Time interval, const std::function<void()> &callback);

private:
    sf::Time m_interval;
    bool m_repeat;
    std::function<void()> m_callback;
    bool m_singleshot;
    bool m_enable;
    sf::Time m_start_time;
};

} // namespace fck

#endif // TIMER_H
