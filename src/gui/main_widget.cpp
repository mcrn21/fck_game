#include "main_widget.h"

#include <spdlog/spdlog.h>

namespace fck::gui
{

MainWidget::MainWidget() : m_render_window{nullptr}, m_hover{nullptr}, m_focus{nullptr}
{
    setSelectable(false);

    WidgetTheme widget_theme = getWidgetTheme();
    widget_theme.padding = 0.0f;
    setWidgetTheme(widget_theme);
}

sf::RenderWindow *MainWidget::getRenderWindow() const
{
    return m_render_window;
}

void MainWidget::setRenderWindow(sf::RenderWindow &render_window)
{
    m_render_window = &render_window;
    Widget::setWindowSize(sf::Vector2f{m_render_window->getSize()});
}

void MainWidget::event(const sf::Event &event)
{
    switch (event.type)
    {
    case sf::Event::MouseMoved: {
        sf::Vector2f mouse = convertMousePosition(event.mouseMove.x, event.mouseMove.y);

        if (m_focus != nullptr && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            m_focus->onMouseMoved(mouse);
            if (!m_focus->getGlobalBounds().contains(mouse))
                m_hover = nullptr;
        }
        else
        {
            Widget *widget = findWidgetAt(mouse);
            widget = widget == this ? nullptr : widget;

            if (m_hover && m_hover == widget)
            {
                m_hover->onMouseMoved(mouse);
                return;
            }

            if (m_hover)
            {
                m_hover->setState(m_focus == m_hover ? WidgetState::FOCUSED : WidgetState::DEFAULT);
                m_hover->onMouseLeave();
                m_hover = nullptr;
            }

            if (widget)
            {
                m_hover = widget;
                m_hover->setState(WidgetState::HOVERED);
                m_hover->onMouseEnter();
            }
        }

        break;
    }

    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse = convertMousePosition(event.mouseButton.x, event.mouseButton.y);

            if (m_focus && m_focus != m_hover)
            {
                m_focus->setState(WidgetState::DEFAULT);
                m_focus = nullptr;
            }

            if (m_hover)
            {
                focusWidget(m_hover);
                m_hover->onMousePressed(mouse);
            }
        }
        break;

    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse = convertMousePosition(event.mouseButton.x, event.mouseButton.y);

            if (m_hover)
            {
                m_hover->onMouseReleased(mouse);

                m_hover->setState(
                    m_hover->getGlobalBounds().contains(mouse)
                        ? WidgetState::HOVERED
                        : (m_hover == m_focus ? WidgetState::FOCUSED : WidgetState::DEFAULT));
            }
        }
        break;

    case sf::Event::MouseWheelScrolled:
        if (m_focus != nullptr)
            m_focus->onMouseWheelMoved(event.mouseWheelScroll.delta);
        break;

    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Tab)
        {
            if (!focusNextWidget())
                focusNextWidget();
        }
        else if (m_focus)
        {
            m_focus->onKeyPressed(event.key);
        }
        break;

    case sf::Event::KeyReleased:
        if (m_focus)
            m_focus->onKeyReleased(event.key);
        break;

    case sf::Event::TextEntered:
        if (m_focus)
            m_focus->onTextEntered(event.text.unicode);
        break;

    case sf::Event::Resized: {
        sf::Vector2f window_szie = {float(event.size.width), float(event.size.height)};
        Widget::setWindowSize(window_szie);
        setSize(window_szie);
        windowResize(this, window_szie);
        break;
    }
    default:
        break;
    }
}

void MainWidget::onDestroyedChild(Widget *widget)
{
    if (m_hover == widget)
        m_hover = nullptr;

    if (m_focus == widget)
        m_focus = nullptr;
}

sf::Vector2f MainWidget::convertMousePosition(int32_t x, int32_t y) const
{
    if (!m_render_window)
        return {float(x), float(y)};

    sf::Vector2f mouse = m_render_window->mapPixelToCoords(sf::Vector2i(x, y));
    mouse -= getPosition();
    return mouse;
}

bool MainWidget::focusWidget(Widget *widget)
{
    if (widget)
    {
        if (m_focus && m_focus != widget)
        {
            m_focus->setState(m_focus == m_hover ? WidgetState::HOVERED : WidgetState::DEFAULT);
            m_focus = nullptr;
        }

        if (widget->isSelectable())
        {
            m_focus = widget;
            widget->setState(WidgetState::FOCUSED);
            return true;
        }
    }

    return false;
}

bool MainWidget::focusNextWidget()
{
    if (!m_focus && !getChildren().empty())
    {
        Widget *widget = findFirstSelectable(this);
        if (widget)
            return focusWidget(widget);
        return false;
    }

    if (m_focus && !m_focus->getChildren().empty())
    {
        Widget *widget = findFirstSelectable(m_focus->getChildren().front());
        if (widget)
            return focusWidget(widget);
        return false;
    }

    Widget *widget = m_focus;
    while (widget && widget->getParent())
    {
        auto parent_children_found = std::find(
            widget->getParent()->getChildren().begin(),
            widget->getParent()->getChildren().end(),
            widget);

        ++parent_children_found;

        if (parent_children_found != widget->getParent()->getChildren().end())
            return focusWidget(*parent_children_found);

        widget = widget->getParent();

        if (!widget->getParent())
            return focusWidget(widget->getChildren().front());
    }

    return false;
}

Widget *MainWidget::findFirstSelectable(Widget *widget)
{
    if (widget->isSelectable())
        return widget;

    for (Widget *child : widget->getChildren())
    {
        Widget *mayby_selectable = findFirstSelectable(child);
        if (mayby_selectable)
            return mayby_selectable;
    }

    return nullptr;
}

void MainWidget::windowResize(Widget *widget, const sf::Vector2f &size)
{
    if (widget)
    {
        widget->onWindowResized(size);
        for (Widget *child : widget->getChildren())
            windowResize(child, size);
    }
}

} // namespace fck::gui
