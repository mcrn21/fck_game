#ifndef MAINMENUGUI_XRRCWSNJBCSE_H
#define MAINMENUGUI_XRRCWSNJBCSE_H

#include "gui_style.h"

#include "gui_elements/gui_elements.h"

#include "../gui_base.h"

#include <functional>
#include <vector>

namespace fck::gui
{

class MainMenuGui : public GuiBase
{
public:
    MainMenuGui(bool level = false);
    ~MainMenuGui() = default;

    void resize(const sf::Vector2f &size);
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

    void onActionActivated(keyboard_action::Action action);

private:
    void createExitDialog();
    void createReturnToMainMenuDialog();

    void enableButtons();
    void disableButtons();

private:
    sf::Vector2f m_viewport_size;
    bool m_level;

    std::vector<Frame *> m_buttons;

    QuestionDialog *m_exit_dialog;
    QuestionDialog *m_return_to_main_menu_dialog;

    sf::RectangleShape m_background;
};

} // namespace fck::gui

#endif // MAINMENUGUI_XRRCWSNJBCSE_H
