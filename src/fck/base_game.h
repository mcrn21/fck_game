#ifndef BASEGAME_KQWRMBUNCVJC_H
#define BASEGAME_KQWRMBUNCVJC_H

#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace fck
{

class BaseGame
{
public:
    BaseGame();
    virtual ~BaseGame();

    int32_t exec();
    void exit();

    sf::RenderWindow &getRrenderWindow();

protected:
    virtual void event(const sf::Event &event);
    virtual void update(const sf::Time &elapsed);
    virtual void draw(const sf::Time &elapsed);

private:
    bool m_running;
    sf::Time m_tick_time;

    sf::RenderWindow m_render_window;
};

} // namespace fck

#endif // BASEGAME_KQWRMBUNCVJC_H
