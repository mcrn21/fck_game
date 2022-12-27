#ifndef MAINWIDGET_CEZTVPEKYBXV_H
#define MAINWIDGET_CEZTVPEKYBXV_H

#include "widget.h"

#include <SFML/Graphics.hpp>

#include <list>
#include <memory>

namespace fck::gui
{

class MainWidget : public Widget
{
public:
    MainWidget();
    ~MainWidget() = default;

    sf::RenderWindow *getRenderWindow() const;
    void setRenderWindow(sf::RenderWindow &render_window);

    void event(const sf::Event &event);

protected:
    void onDestroyedChild(Widget *widget);

private:
    sf::Vector2f convertMousePosition(int32_t x, int32_t y) const;

    bool focusWidget(Widget *widget);
    bool focusNextWidget();
    Widget *findFirstSelectable(Widget *widget);

    void windowResize(Widget *widget, const sf::Vector2f &size);

private:
    sf::RenderWindow *m_render_window;

    Widget *m_hover;
    Widget *m_focus;
};

} // namespace fck::gui

#endif // MAINWIDGET_CEZTVPEKYBXV_H
