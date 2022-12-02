#ifndef SPLASHSCREENGUI_H
#define SPLASHSCREENGUI_H

#include "../gui_base.h"

#include <SFML/Graphics.hpp>

namespace fck
{

class SplashscreenGui : public GuiBase
{
public:
    SplashscreenGui(sf::View *view);
    ~SplashscreenGui() = default;

    void draw();

private:
    sf::View *m_view;
    sf::Texture *m_spalshscreen_texture;
};

} // namespace fck

#endif // SPLASHSCREENGUI_H
