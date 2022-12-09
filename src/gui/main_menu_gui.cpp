#include "main_menu_gui.h"

#include "../fck/event_dispatcher.h"

namespace fck::gui
{

MainMenuGui::MainMenuGui(const sf::Vector2f &size, bool level) : m_size{size}, m_level{level}
{
    if (!m_level)
    {
        m_main_menu_entries.push_back(
            {"New game", []() { EventDispatcher::push(event_type::NEW_GAME); }});
        m_main_menu_entries.push_back({"Load game", []() {}});
        m_main_menu_entries.push_back({"Settings", []() {}});
        m_main_menu_entries.push_back(
            {"Exit",
             [this]() { m_dialogs.push_back(std::bind(&MainMenuGui::drawExitGameDialog, this)); }});
    }
    else
    {
        m_main_menu_entries.push_back(
            {"Return to game", []() { EventDispatcher::push(event_type::RETURN_TO_GAME); }});
        m_main_menu_entries.push_back(
            {"Return to main menu", [this]() {
                 m_dialogs.push_back(std::bind(&MainMenuGui::drawReturnToMainMenuDialog, this));
             }});
        m_main_menu_entries.push_back({"Settings", []() {}});
        m_main_menu_entries.push_back(
            {"Exit",
             [this]() { m_dialogs.push_back(std::bind(&MainMenuGui::drawExitGameDialog, this)); }});
    }
}

void MainMenuGui::resize(const sf::Vector2f &size)
{
    m_size = size;
}

void MainMenuGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(m_size);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f));

    ImGui::Begin(
        "Main menu screen", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    ImGui::BeginDisabled(!m_dialogs.empty());

    static ImVec2 button_size(210, gui::FONT_SIZE /* * 1.2*/);
    static int32_t screen_offset = 20;
    static int32_t button_space = 5;

    for (int32_t i = m_main_menu_entries.size() - 1; i >= 0; --i)
    {
        ImGui::SetCursorScreenPos(ImVec2(
            screen_offset,
            m_size.y - screen_offset - button_size.y * (m_main_menu_entries.size() - i)
                - button_space * (m_main_menu_entries.size() - 1 - i)));

        bool selected = false;
        if (ImGui::Selectable(m_main_menu_entries[i].name.c_str(), &selected, 0, button_size))
            m_main_menu_entries[i].callback();
    }

    ImGui::EndDisabled();

    ImGui::End();
    ImGui::PopStyleVar(3);

    if (!m_dialogs.empty())
    {
        for (int32_t i = 0; i < m_dialogs.size(); ++i)
        {
            bool enable = i == m_dialogs.size() - 1;
            ImGui::BeginDisabled(!enable);
            m_dialogs[i]();
            ImGui::EndDisabled();
        }
    }

    ImGui::PopFont();
}

void MainMenuGui::onActionActivated(keyboard_action::Action action)
{
    if (action == keyboard_action::BACK)
    {
        if (m_dialogs.empty())
        {
            if (m_level)
                EventDispatcher::push(event_type::RETURN_TO_GAME);
        }
        else
        {
            m_dialogs.pop_back();
        }
    }
}

void MainMenuGui::drawExitGameDialog()
{
    ImGui::Begin("Exit");

    ImGui::Text("Exit from game?");

    if (ImGui::Button("Yes"))
    {
        EventDispatcher::push(event_type::EXIT_GAME);
    }
    ImGui::SameLine();
    if (ImGui::Button("No"))
    {
        m_dialogs.pop_back();
    }

    ImGui::End();
}

void MainMenuGui::drawReturnToMainMenuDialog()
{
    ImGui::Begin("Return to main menu");

    ImGui::Text("The gameplay will be lost, are you sure?");

    if (ImGui::Button("Yes"))
    {
        EventDispatcher::push(event_type::RETURN_TO_MAIN_MENU);
    }
    ImGui::SameLine();
    if (ImGui::Button("No"))
    {
        m_dialogs.pop_back();
    }

    ImGui::End();
}

} // namespace fck::gui
