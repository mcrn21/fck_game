#ifndef MAINMENUGUI_H
#define MAINMENUGUI_H

#include "../gui_base.h"

#include <functional>
#include <vector>

namespace fck
{

class MainMenuGui : public GuiBase
{
public:
    MainMenuGui(const sf::Vector2f &size, bool level = false);
    ~MainMenuGui() = default;

    void resize(const sf::Vector2f &size);
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

    void onActionActivated(keyboard_action::Action action);

private:
    void drawExitGameDialog();
    void drawReturnToMainMenuDialog();

private:
    sf::Vector2f m_size;
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
