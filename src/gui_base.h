#ifndef GUIBASE_H
#define GUIBASE_H

#include "fck_common.h"

#include <imgui-SFML.h>
#include <imgui.h>

namespace fck
{

class GuiBase
{
public:
    GuiBase() = default;
    virtual ~GuiBase() = default;

    virtual void draw() = 0;

public:
    static ImFont *main_menu_font;
    static ImFont *hp_hud_font;
};

} // namespace fck

#endif // GUIBASE_H
