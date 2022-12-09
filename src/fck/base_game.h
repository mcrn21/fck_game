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

    static BaseGame *instance();

    int32_t exec();
    void exit();

    sf::RenderWindow &renderWindow();

protected:
    virtual void update(const sf::Time &elapsed);
    virtual void draw(const sf::Time &elapsed);

private:
    static BaseGame *m_instance;

    bool m_running;
    sf::Time m_tick_time;

    sf::RenderWindow m_render_window;
};

} // namespace fck

#endif // BASEGAME_KQWRMBUNCVJC_H
