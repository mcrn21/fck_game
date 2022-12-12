#include "base_game.h"
#include "event_dispatcher.h"

#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>

namespace fck
{

BaseGame *BaseGame::m_instance = nullptr;

BaseGame::BaseGame() : m_running{false}, m_tick_time{sf::milliseconds(16)}
{
    assert(!BaseGame::m_instance);
    m_instance = this;

    m_render_window.setVerticalSyncEnabled(false);
}

BaseGame::~BaseGame()
{
}

BaseGame *BaseGame::instance()
{
    return m_instance;
}

int32_t BaseGame::exec()
{
    m_running = true;

    sf::Clock loop_clock;
    sf::Time elapsed = sf::milliseconds(0);
    sf::Time lag = sf::milliseconds(0);

    while (m_running)
    {
        lag += (elapsed = loop_clock.restart());

        sf::Event event;
        while (m_render_window.pollEvent(event))
        {
            EventDispatcher::push(new SfmlEvent{event});
        }

        EventDispatcher::update(elapsed);

        while (lag >= m_tick_time)
        {
            update(m_tick_time);
            lag -= m_tick_time;
        }

        draw(elapsed);
    }

    return 0;
}

void BaseGame::exit()
{
    spdlog::info("Exit game");
    m_running = false;
}

sf::RenderWindow &BaseGame::renderWindow()
{
    return m_render_window;
}

void BaseGame::update(const sf::Time &elapsed)
{
    (void)(elapsed);
}

void BaseGame::draw(const sf::Time &elapsed)
{
    m_render_window.clear();
    m_render_window.display();
}

} // namespace fck
