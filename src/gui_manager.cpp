#include "gui_manager.h"

namespace fck
{

GuiManager::GuiManager()
    : EventHandler{
        {sf::Event::TextEntered,
         sf::Event::KeyPressed,
         sf::Event::KeyReleased,
         sf::Event::MouseWheelScrolled,
         sf::Event::MouseButtonPressed,
         sf::Event::MouseButtonReleased,
         sf::Event::MouseMoved},
        {}}
{
}

void GuiManager::setViewportSize(const sf::Vector2f &viewport_size)
{
    m_viewport_size = viewport_size;
    for (const auto &gui : m_gui_list)
        gui->resize(viewport_size);
}

void GuiManager::push(GuiBase *gui)
{
    m_gui_list.push_back(std::unique_ptr<GuiBase>(gui));
}

void GuiManager::pop()
{
    m_gui_list.pop_back();
}

GuiBase *GuiManager::getBack() const
{
    if (m_gui_list.empty())
        return nullptr;
    return m_gui_list.back().get();
}

void GuiManager::clear()
{
    m_gui_list.clear();
}

void GuiManager::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    for (const auto &gui : m_gui_list)
        gui->draw(target, states);
}

void GuiManager::event(Event *event)
{
    if (!m_gui_list.empty())
        m_gui_list.back()->event(event);
}

} // namespace fck
