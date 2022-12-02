#include "base_game.h"
#include "event_dispatcher.h"

#include <spdlog/spdlog.h>
#include <SFML/Window/Event.hpp>

namespace fck
{

const std::vector<sf::Event::EventType> SFML_INPUT_EVENTS
    = {sf::Event::MouseMoved,
       sf::Event::MouseButtonPressed,
       sf::Event::MouseButtonReleased,
       sf::Event::TouchBegan,
       sf::Event::TouchEnded,
       sf::Event::MouseWheelScrolled,
       sf::Event::KeyPressed,
       sf::Event::KeyReleased,
       sf::Event::TextEntered};

BaseGame *BaseGame::m_instance = nullptr;

BaseGame::BaseGame() : m_running{false}, m_tick_time{sf::milliseconds(16)}
{
    assert(!BaseGame::m_instance);
    m_instance = this;

    m_render_window.setVerticalSyncEnabled(false);

    ImGui::SFML::Init(m_render_window);
}

BaseGame::~BaseGame()
{
    ImGui::SFML::Shutdown();
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
            ImGui::SFML::ProcessEvent(m_render_window, event);

            if (!(ImGui::IsAnyItemActive()
                  && std::find(SFML_INPUT_EVENTS.begin(), SFML_INPUT_EVENTS.end(), event.type)
                      == SFML_INPUT_EVENTS.end()))
            {
                EventDispatcher::push(new SfmlEvent{event});
            }
        }

        EventDispatcher::update(elapsed);

        while (lag >= m_tick_time)
        {
            update(m_tick_time);
            lag -= m_tick_time;
        }

        ImGui::SFML::Update(m_render_window, elapsed);
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

    ImGui::SFML::Render(m_render_window);

    m_render_window.display();
}

} // namespace fck
