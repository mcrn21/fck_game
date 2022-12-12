#include "main_menu_gui.h"

#include "../fck/event_dispatcher.h"

namespace fck::gui
{

MainMenuGui::MainMenuGui(bool level)
    : m_level{level}, m_exit_dialog{nullptr}, m_return_to_main_menu_dialog{nullptr}
{
    if (!m_level)
    {
        Button *new_game_button = Style::createMainMenuButton("New game");
        new_game_button->clicked.connect([]() { EventDispatcher::push(event_type::NEW_GAME); });
        m_buttons.push_back(new_game_button);

        Button *load_game_button = Style::createMainMenuButton("Load game");
        m_buttons.push_back(load_game_button);

        Button *settings_button = Style::createMainMenuButton("Settings");
        m_buttons.push_back(settings_button);

        Button *exit_button = Style::createMainMenuButton("Exit");
        exit_button->clicked.connect([this]() { createExitDialog(); });
        m_buttons.push_back(exit_button);
    }
    else
    {
        Button *return_to_game_button = Style::createMainMenuButton("Return to game");
        return_to_game_button->clicked.connect(
            []() { EventDispatcher::push(event_type::RETURN_TO_GAME); });
        m_buttons.push_back(return_to_game_button);

        Button *return_to_main_menu_button = Style::createMainMenuButton("Return to main menu");
        return_to_main_menu_button->clicked.connect([this]() { createReturnToMainMenuDialog(); });
        m_buttons.push_back(return_to_main_menu_button);

        Button *settings_button = Style::createMainMenuButton("Settings");
        m_buttons.push_back(settings_button);

        Button *exit_button = Style::createMainMenuButton("Exit");
        exit_button->clicked.connect([this]() { createExitDialog(); });
        m_buttons.push_back(exit_button);
    }

    for (Frame *button : m_buttons)
        addFrame(button);

    m_background.setFillColor(sf::Color{0, 0, 0, 127});
    m_background.setPosition({0.0f, 0.0f});
}

void MainMenuGui::resize(const sf::Vector2f &size)
{
    m_viewport_size = size;

    for (int32_t i = m_buttons.size() - 1; i >= 0; --i)
    {
        m_buttons[m_buttons.size() - 1 - i]->setPosition(
            {Style::viewport_offset.x,
             size.y - Style::viewport_offset.y
                 - m_buttons[m_buttons.size() - 1 - i]->getGlobalBounds().getSize().y * (i + 1)});
    }

    m_background.setSize(size);

    if (m_exit_dialog)
        m_exit_dialog->setPosition(size / 2.0f - m_exit_dialog->getGlobalBounds().getSize() / 2.0f);

    if (m_return_to_main_menu_dialog)
    {
        m_return_to_main_menu_dialog->setPosition(
            size / 2.0f - m_return_to_main_menu_dialog->getGlobalBounds().getSize() / 2.0f);
    }
}

void MainMenuGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_background);
    GuiBase::draw(target, states);
}

void MainMenuGui::createExitDialog()
{
    disableButtons();
    m_exit_dialog = Style::createQuestionDialog(L"Exit from game?", "Yes", "No");

    m_exit_dialog->setPosition(
        m_viewport_size / 2.0f - m_exit_dialog->getGlobalBounds().getSize() / 2.0f);

    m_exit_dialog->cancelButton()->clicked.connect([this]() {
        EventDispatcher::runTask([this]() {
            delete m_exit_dialog;
            m_exit_dialog = nullptr;
            enableButtons();
        });
    });

    m_exit_dialog->okButton()->clicked.connect(
        [this]() { EventDispatcher::push(event_type::EXIT_GAME); });

    addFrame(m_exit_dialog);
}

void MainMenuGui::createReturnToMainMenuDialog()
{
    disableButtons();
    m_return_to_main_menu_dialog
        = Style::createQuestionDialog(L"Return to main menu?", "Yes", "No");

    m_return_to_main_menu_dialog->setPosition(
        m_viewport_size / 2.0f - m_return_to_main_menu_dialog->getGlobalBounds().getSize() / 2.0f);

    m_return_to_main_menu_dialog->cancelButton()->clicked.connect([this]() {
        EventDispatcher::runTask([this]() {
            delete m_return_to_main_menu_dialog;
            m_return_to_main_menu_dialog = nullptr;
            enableButtons();
        });
    });

    m_return_to_main_menu_dialog->okButton()->clicked.connect(
        [this]() { EventDispatcher::push(event_type::RETURN_TO_MAIN_MENU); });

    addFrame(m_return_to_main_menu_dialog);
}

void MainMenuGui::enableButtons()
{
    for (Frame *button : m_buttons)
        button->setEnable(true);
}

void MainMenuGui::disableButtons()
{
    for (Frame *button : m_buttons)
        button->setEnable(false);
}

void MainMenuGui::onActionActivated(keyboard_action::Action action)
{
    if (action == keyboard_action::BACK)
    {
        if (m_level)
            EventDispatcher::push(event_type::RETURN_TO_GAME);
    }
}

} // namespace fck::gui
