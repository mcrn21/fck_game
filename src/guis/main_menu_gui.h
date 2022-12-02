#ifndef MAINMENUGUI_H
#define MAINMENUGUI_H

#include "../gui_base.h"

#include <SFML/Graphics.hpp>

#include <functional>
#include <vector>

namespace fck
{

class MainMenuGui : public GuiBase
{
public:
    MainMenuGui(sf::View *view, bool level = false);
    ~MainMenuGui() = default;

    void draw();

    void onActionActivated(keyboard_action::Action action);

private:
    void drawExitGameDialog();
    void drawReturnToMainMenuDialog();

private:
    sf::View *m_view;
    bool m_level;

    struct MenuEntry
    {
        std::string name;
        std::function<void()> callback;
    };

    std::vector<MenuEntry> m_main_menu_entries;
    std::vector<std::function<void()>> m_dialogs;
};

} // namespace fck

#endif // MAINMENUGUI_H
