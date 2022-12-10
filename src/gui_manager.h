#ifndef GUIMANAGER_NYZESRRUUOKE_H
#define GUIMANAGER_NYZESRRUUOKE_H

#include "gui_base.h"

#include "fck/event_handler.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace fck
{

class GuiManager : public EventHandler
{
public:
    GuiManager();
    ~GuiManager() = default;

    void setViewportSize(const sf::Vector2f &viewport_size);

    void push(GuiBase *gui);

    template<typename T, typename... Args>
    T *push(Args &&...args)
    {
        T *gui = new T{std::forward<Args>(args)...};
        gui->resize(m_viewport_size);
        push(gui);
        return gui;
    }

    void pop();

    GuiBase *back() const;

    template<typename T>
    T *back() const
    {
        return static_cast<T *>(back());
    }

    void clear();

    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

protected:
    void event(Event *event);

private:
    sf::Vector2f m_viewport_size;
    std::vector<std::unique_ptr<GuiBase>> m_gui_list;
};

} // namespace fck

#endif // GUIMANAGER_NYZESRRUUOKE_H
