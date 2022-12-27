#include "base_game.h"

#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>

namespace fck
{

BaseGame::BaseGame() : m_running{false}, m_tick_time{sf::milliseconds(16)}
{
    m_render_window.setVerticalSyncEnabled(false);
}

BaseGame::~BaseGame()
{
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

        sf::Event e;
        while (m_render_window.pollEvent(e))
            event(e);

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

sf::RenderWindow &BaseGame::getRrenderWindow()
{
    return m_render_window;
}

void BaseGame::event(const sf::Event &event)
{
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
