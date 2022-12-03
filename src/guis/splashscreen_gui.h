#ifndef SPLASHSCREENGUI_H
#define SPLASHSCREENGUI_H

#include "../gui_base.h"

namespace fck
{

class SplashscreenGui : public GuiBase
{
public:
    SplashscreenGui(const sf::Vector2f &size);
    ~SplashscreenGui() = default;

    void resize(const sf::Vector2f &size);
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

private:
    sf::Vector2f m_size;
    sf::Sprite m_splashsceen_sprite;
};

} // namespace fck

#endif // SPLASHSCREENGUI_H
