#ifndef GUIBASE_H
#define GUIBASE_H

#include "fck_common.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>

namespace fck
{

class GuiBase
{
public:
    GuiBase() = default;
    virtual ~GuiBase() = default;

    virtual void resize(const sf::Vector2f &size) = 0;
    virtual void draw(sf::RenderTarget &target, const sf::RenderStates &states) = 0;

public:
    static ImFont *main_menu_font;
    static ImFont *hp_hud_font;
};

} // namespace fck

#endif // GUIBASE_H
