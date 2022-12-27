#include "main_menu_widget.h"

#include "../../fck/event_dispatcher.h"

namespace fck::gui
{

MainMenuWidget::MainMenuWidget(bool level, Widget *parent) : Widget{parent}, m_level{level}
{
    WidgetTheme widget_theme = getWidgetTheme();
    widget_theme.padding = 0.0f;
    setWidgetTheme(widget_theme);

    setSize(Widget::getWindowSize());
    setSelectable(false);

    m_background.setFillColor(sf::Color{0, 0, 0, 127});
    m_background.setPosition({0.0f, 0.0f});

    sf::Vector2f main_button_size = {200.0f, 50.0f};
    WidgetTheme button_theme = WidgetTheme::get<Button>();
    button_theme.background.texture_rects[WidgetState::DEFAULT] = {};
    button_theme.text.align = TextAlign::LEFT | TextAlign::V_CENTER;

    if (!m_level)
    {
        gui::Button *new_game_button = new gui::Button{L"New game", this};
        new_game_button->setSize(main_button_size);
        new_game_button->setWidgetTheme(button_theme);
        new_game_button->clicked.connect([]() { EventDispatcher::push(event_type::NEW_GAME); });
        m_main_buttons.push_back(new_game_button);

        Button *load_game_button = new gui::Button{L"Load game", this};
        load_game_button->setSize(main_button_size);
        load_game_button->setWidgetTheme(button_theme);
        m_main_buttons.push_back(load_game_button);

        Button *settings_button = new gui::Button{L"Settings", this};
        settings_button->setSize(main_button_size);
        settings_button->setWidgetTheme(button_theme);
        m_main_buttons.push_back(settings_button);

        Button *exit_button = new gui::Button{L"Exit", this};
        exit_button->setSize(main_button_size);
        exit_button->setWidgetTheme(button_theme);
        exit_button->clicked.connect([this]() { showExitDialog(); });
        m_main_buttons.push_back(exit_button);
    }
    else
    {
        Button *return_to_game_button = new gui::Button{L"Return to game", this};
        return_to_game_button->setSize(main_button_size);
        return_to_game_button->setWidgetTheme(button_theme);
        return_to_game_button->clicked.connect(
            []() { EventDispatcher::push(event_type::RETURN_TO_GAME); });
        m_main_buttons.push_back(return_to_game_button);

        Button *return_to_main_menu_button = new gui::Button{L"Return to main menu", this};
        return_to_main_menu_button->setSize(main_button_size);
        return_to_main_menu_button->setWidgetTheme(button_theme);
        return_to_main_menu_button->clicked.connect([this]() { showReturnToMainMenuDialog(); });
        m_main_buttons.push_back(return_to_main_menu_button);

        Button *settings_button = new gui::Button{L"Settings", this};
        settings_button->setSize(main_button_size);
        settings_button->setWidgetTheme(button_theme);
        m_main_buttons.push_back(settings_button);

        Button *exit_button = new gui::Button{L"Exit", this};
        exit_button->setSize(main_button_size);
        exit_button->setWidgetTheme(button_theme);
        exit_button->clicked.connect([this]() { showExitDialog(); });
        m_main_buttons.push_back(exit_button);
    }

    // exit dialog
    m_exit_dialog = new QuestionDialog{L"Exit from game?", L"Exit from game?", L"Yes", L"No", this};
    m_exit_dialog->setDialoFlags(0);
    m_exit_dialog->setSize({250.0f, 170.0f});
    m_exit_dialog->getYesButton()->clicked.connect(
        []() { EventDispatcher::push(event_type::EXIT_GAME); });
    m_exit_dialog->getNoButton()->clicked.connect([this]() { m_exit_dialog->hide(); });

    // return to main menu dialog
    m_return_to_main_menu_dialog
        = new QuestionDialog{L"Return to main menu?", L"Return to main menu?", L"Yes", L"No", this};
    m_return_to_main_menu_dialog->setDialoFlags(0);
    m_return_to_main_menu_dialog->setSize({350.0f, 170.0f});
    m_return_to_main_menu_dialog->getYesButton()->clicked.connect(
        []() { EventDispatcher::push(event_type::RETURN_TO_MAIN_MENU); });
    m_return_to_main_menu_dialog->getNoButton()->clicked.connect(
        [this]() { m_return_to_main_menu_dialog->hide(); });

    updateGeometry();
}

void MainMenuWidget::onActionActivated(keyboard_action::Action action)
{
}

void MainMenuWidget::onWindowResized(const sf::Vector2f &size)
{
    setSize(size);
    updateGeometry();
}

void MainMenuWidget::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= getTransform();

    target.draw(m_background, new_states);

    drawChildren(target, new_states);
}

void MainMenuWidget::showExitDialog()
{
    m_exit_dialog->show();
    m_exit_dialog->setPosition((Widget::getWindowSize() - m_exit_dialog->getSize()) / 2.0f);
}

void MainMenuWidget::showReturnToMainMenuDialog()
{
    m_return_to_main_menu_dialog->show();
    m_return_to_main_menu_dialog->setPosition(
        (Widget::getWindowSize() - m_return_to_main_menu_dialog->getSize()) / 2.0f);
}

void MainMenuWidget::updateGeometry()
{
    Sides<float> viewport_padding = 30.0f;

    m_background.setSize(getSize());

    for (int32_t i = m_main_buttons.size() - 1; i >= 0; --i)
    {
        m_main_buttons[m_main_buttons.size() - 1 - i]->setPosition(
            {viewport_padding.left,
             getSize().y - viewport_padding.top
                 - m_main_buttons[m_main_buttons.size() - 1 - i]->getGlobalBounds().getSize().y
                     * (i + 1)});
    }
}

} // namespace fck::gui
