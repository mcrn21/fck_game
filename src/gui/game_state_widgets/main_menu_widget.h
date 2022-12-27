#ifndef MAINMENUWIDGET_ERVAVORMPYWG_H
#define MAINMENUWIDGET_ERVAVORMPYWG_H

#include "../button.h"
#include "../dialog.h"
#include "../label.h"
#include "../widget.h"

#include "../../fck_common.h"

#include <SFML/Graphics.hpp>

#include <vector>

namespace fck::gui
{

class MainMenuWidget : public Widget
{
public:
    MainMenuWidget(bool level = false, Widget *parent = nullptr);
    ~MainMenuWidget() = default;

    void onActionActivated(keyboard_action::Action action);

protected:
    void onWindowResized(const sf::Vector2f &size) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void showExitDialog();
    void showReturnToMainMenuDialog();
    void updateGeometry();

private:
    bool m_level;

    sf::RectangleShape m_background;
    std::vector<Button *> m_main_buttons;
    QuestionDialog *m_exit_dialog;
    QuestionDialog *m_return_to_main_menu_dialog;
};

} // namespace fck::gui

#endif // MAINMENUWIDGET_ERVAVORMPYWG_H
