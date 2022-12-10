#ifndef SPLASHSCREENGUI_NPJFUWSQVWEV_H
#define SPLASHSCREENGUI_NPJFUWSQVWEV_H

#include "../gui_base.h"

namespace fck::gui
{

class SplashscreenGui : public GuiBase
{
public:
    SplashscreenGui();
    ~SplashscreenGui() = default;

    void resize(const sf::Vector2f &size);
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

private:
    sf::Sprite m_splashsceen_sprite;
};

} // namespace fck::gui

#endif // SPLASHSCREENGUI_NPJFUWSQVWEV_H
