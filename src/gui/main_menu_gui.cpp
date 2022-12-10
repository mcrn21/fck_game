#include "main_menu_gui.h"

#include "../fck/event_dispatcher.h"

namespace fck::gui
{

MainMenuGui::MainMenuGui(bool level)
    : m_level{level}, m_exit_dialog{nullptr}, m_return_to_main_menu_dialog{nullptr}
{
    if (!m_level)
    {
        Button *new_game_button = GuiStyle::createMainMenuButton("New game");
        new_game_button->clicked.connect([]() { EventDispatcher::push(event_type::NEW_GAME); });
        m_buttons.push_back(new_game_button);

        Button *load_game_button = GuiStyle::createMainMenuButton("Load game");
        m_buttons.push_back(load_game_button);

        Button *settings_button = GuiStyle::createMainMenuButton("Settings");
        m_buttons.push_back(settings_button);

        Button *exit_button = GuiStyle::createMainMenuButton("Exit");
        exit_button->clicked.connect([this]() { createExitDialog(); });
        m_buttons.push_back(exit_button);
    }
    else
    {
        Button *return_to_game_button = GuiStyle::createMainMenuButton("Return to game");
        return_to_game_button->clicked.connect(
            []() { EventDispatcher::push(event_type::RETURN_TO_GAME); });
        m_buttons.push_back(return_to_game_button);

        Button *return_to_main_menu_button = GuiStyle::createMainMenuButton("Return to main menu");
        return_to_main_menu_button->clicked.connect([this]() { createReturnToMainMenuDialog(); });
        m_buttons.push_back(return_to_main_menu_button);

        Button *settings_button = GuiStyle::createMainMenuButton("Settings");
        m_buttons.push_back(settings_button);

        Button *exit_button = GuiStyle::createMainMenuButton("Exit");
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
            {GuiStyle::viewport_offset.x,
             size.y - GuiStyle::viewport_offset.y
                 - m_buttons[m_buttons.size() - 1 - i]->getGlobalBounds().getSize().y * (i + 1)});
    }

    m_background.setSize(size);
}

void MainMenuGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_background);
    GuiBase::draw(target, states);
}

void MainMenuGui::createExitDialog()
{
    disableButtons();
    m_exit_dialog = GuiStyle::createQuestionDialog(
        L"Run паца run\nCuz im already done\nRun паца run\nWtf with you, man", "Yes", "No");

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

//void MainMenuGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
//{
//    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

//    ImGui::SetNextWindowPos(ImVec2(0, 0));
//    ImGui::SetNextWindowSize(m_size);

//    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
//    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));

//    ImGui::Begin(
//        "Main menu screen", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

//    ImGui::BeginDisabled(!m_dialogs.empty());

//    static ImVec2 button_size(210, gui::FONT_SIZE /* * 1.2*/);
//    static int32_t screen_offset = 20;
//    static int32_t button_space = 5;

//    for (int32_t i = m_main_menu_entries.size() - 1; i >= 0; --i)
//    {
//        ImGui::SetCursorScreenPos(ImVec2(
//            screen_offset,
//            m_size.y - screen_offset - button_size.y * (m_main_menu_entries.size() - i)
//                - button_space * (m_main_menu_entries.size() - 1 - i)));

//        bool selected = false;
//        if (ImGui::Selectable(m_main_menu_entries[i].name.c_str(), &selected, 0, button_size))
//            m_main_menu_entries[i].callback();
//    }

//    ImGui::EndDisabled();

//    ImGui::End();
//    ImGui::PopStyleVar(3);

//    if (!m_dialogs.empty())
//    {
//        for (int32_t i = 0; i < m_dialogs.size(); ++i)
//        {
//            bool enable = i == m_dialogs.size() - 1;
//            ImGui::BeginDisabled(!enable);
//            m_dialogs[i]();
//            ImGui::EndDisabled();
//        }
//    }

//    ImGui::PopFont();
//}

void MainMenuGui::onActionActivated(keyboard_action::Action action)
{
    //    if (action == keyboard_action::BACK)
    //    {
    //        if (m_dialogs.empty())
    //        {
    //            if (m_level)
    //                EventDispatcher::push(event_type::RETURN_TO_GAME);
    //        }
    //        else
    //        {
    //            m_dialogs.pop_back();
    //        }
    //    }
}

void MainMenuGui::drawExitGameDialog()
{
    //    ImGui::Begin("Exit");

    //    ImGui::Text("Exit from game?");

    //    if (ImGui::Button("Yes"))
    //    {
    //        EventDispatcher::push(event_type::EXIT_GAME);
    //    }
    //    ImGui::SameLine();
    //    if (ImGui::Button("No"))
    //    {
    //        m_dialogs.pop_back();
    //    }

    //    ImGui::End();
}

void MainMenuGui::drawReturnToMainMenuDialog()
{
    //    ImGui::Begin("Return to main menu");

    //    ImGui::Text("The gameplay will be lost, are you sure?");

    //    if (ImGui::Button("Yes"))
    //    {
    //        EventDispatcher::push(event_type::RETURN_TO_MAIN_MENU);
    //    }
    //    ImGui::SameLine();
    //    if (ImGui::Button("No"))
    //    {
    //        m_dialogs.pop_back();
    //    }

    //    ImGui::End();
}

} // namespace fck::gui
