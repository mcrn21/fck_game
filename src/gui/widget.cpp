#include "widget.h"

namespace fck::gui
{

sf::Vector2f Widget::m_window_size;

Widget::Widget(Widget *parent)
    : m_parent{nullptr},
      m_state{WidgetState::DEFAULT},
      m_selectable{true},
      m_show{true},
      m_enable{true},
      m_widget_theme{WidgetTheme::get<Widget>()}
{
    setParent(parent);
}

Widget::~Widget()
{
    std::vector<Widget *> children = m_children;
    for (Widget *child : children)
        delete child;
    if (m_parent)
        m_parent->onDestroyedChild(this);
    setParent(nullptr);
}

const sf::Vector2f &Widget::getPosition() const
{
    return m_position;
}

void Widget::setPosition(const sf::Vector2f &position)
{
    sf::Vector2f delta = position - m_position;
    m_position = position;
    m_transform.translate(delta);
}

const sf::Transform &Widget::getTransform() const
{
    return m_transform;
}

const sf::Vector2f &Widget::getSize() const
{
    return m_size;
}

void Widget::setSize(const sf::Vector2f &size)
{
    m_size = size;
    onResized(m_size);
}

sf::FloatRect Widget::getLocalBounds() const
{
    return sf::FloatRect{{0.0f, 0.0f}, m_size};
}

sf::FloatRect Widget::getGlobalBounds() const
{
    sf::Vector2f position = getPosition();

    Widget *parent = m_parent;
    while (parent)
    {
        position += parent->getPosition()
            + sf::Vector2f{parent->m_widget_theme.padding.left, parent->m_widget_theme.padding.top};
        parent = parent->m_parent;
    }

    return sf::FloatRect{position, m_size};
}

Widget *Widget::getParent() const
{
    return m_parent;
}

void Widget::setParent(Widget *widget)
{
    if (m_parent)
    {
        m_parent->m_children.erase(
            std::remove(m_parent->m_children.begin(), m_parent->m_children.end(), this),
            m_parent->m_children.end());
    }

    m_parent = widget;

    if (m_parent)
        m_parent->m_children.push_back(this);
}

const std::vector<Widget *> &Widget::getChildren() const
{
    return m_children;
}

void Widget::removeLastChild()
{
    if (m_children.empty())
        return;
    delete m_children.back();
}

void Widget::clearChildren()
{
    for (Widget *child : m_children)
        delete child;
    m_children.clear();
}

WidgetState Widget::getState() const
{
    return m_state;
}

bool Widget::isFocused() const
{
    return m_state == WidgetState::FOCUSED || m_state == WidgetState::PRESSED;
}

bool Widget::isSelectable() const
{
    return m_selectable;
}

void Widget::setSelectable(bool selectable)
{
    m_selectable = selectable;
    for (Widget *child : m_children)
        child->setSelectable(m_selectable);
}

bool Widget::isShow() const
{
    return m_show;
}

void Widget::show()
{
    m_show = true;
}

void Widget::hide()
{
    m_show = false;
}

bool Widget::isEnable() const
{
    return m_enable;
}

void Widget::setEnable(bool enable)
{
    m_enable = enable;
    for (Widget *child : m_children)
        child->setEnable(m_enable);
}

const WidgetTheme &Widget::getWidgetTheme() const
{
    return m_widget_theme;
}

void Widget::setWidgetTheme(const WidgetTheme &widget_theme, bool children)
{
    m_widget_theme = widget_theme;
    onThemeChanged(m_widget_theme);

    if (children)
    {
        for (Widget *child : m_children)
            child->setWidgetTheme(widget_theme, true);
    }
}

sf::Vector2f Widget::getChildrenSize() const
{
    sf::Vector2f size;

    sf::Vector2f lower_point
        = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    sf::Vector2f upper_point
        = {std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};

    for (Widget *child : m_children)
    {
        if (child->getPosition().x < lower_point.x)
            lower_point.x = child->getPosition().x;

        if (child->getPosition().y < lower_point.y)
            lower_point.y = child->getPosition().y;

        if ((child->getPosition().x + child->getSize().x) > upper_point.x)
            upper_point.x = child->getPosition().x + child->getSize().x;

        if ((child->getPosition().y + child->getSize().y) > upper_point.y)
            upper_point.y = child->getPosition().x + child->getSize().y;

        size = upper_point - lower_point;
    }

    return size;
}

sf::Vector2f Widget::getContentSize() const
{
    sf::Vector2f size = getChildrenSize();

    size += {m_widget_theme.padding.left, m_widget_theme.padding.top};
    size += {m_widget_theme.padding.right, m_widget_theme.padding.bottom};

    return size;
}

const sf::Vector2f &Widget::getWindowSize()
{
    return m_window_size;
}

void Widget::setWindowSize(const sf::Vector2f &window_Size)
{
    m_window_size = window_Size;
}

void Widget::onMouseEnter()
{
}

void Widget::onMouseLeave()
{
}

void Widget::onMouseMoved(const sf::Vector2f &mouse_position)
{
}

void Widget::onMousePressed(const sf::Vector2f &mouse_position)
{
}

void Widget::onMouseReleased(const sf::Vector2f &mouse_position)
{
}

void Widget::onMouseWheelMoved(int32_t delta)
{
}

void Widget::onKeyPressed(const sf::Event::KeyEvent &key)
{
}

void Widget::onKeyReleased(const sf::Event::KeyEvent &key)
{
}

void Widget::onTextEntered(uint32_t unicode)
{
}

void Widget::onDestroyedChild(Widget *widget)
{
    if (m_parent)
        m_parent->onDestroyedChild(widget);
}

void Widget::onStateChanged(WidgetState state)
{
}

void Widget::onResized(const sf::Vector2f &size)
{
}

void Widget::onWindowResized(const sf::Vector2f &size)
{
}

void Widget::onThemeChanged(const WidgetTheme &widget_theme)
{
}

void Widget::setState(WidgetState state)
{
    m_state = state;
    onStateChanged(m_state);
}

Widget *Widget::findWidgetAt(const sf::Vector2f &position)
{
    Widget *widget = nullptr;

    for (Widget *child : m_children)
    {
        if (child->isShow() && child->getGlobalBounds().contains(position))
            widget = child->findWidgetAt(position);
    }

    return widget == nullptr ? this : widget;
}

void Widget::drawChildren(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform.translate({m_widget_theme.padding.left, m_widget_theme.padding.top});

    for (Widget *child : m_children)
    {
        if (child->isShow())
            target.draw(*child, new_states);
    }
}

void Widget::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= m_transform;

    drawChildren(target, new_states);
}

} // namespace fck::gui
