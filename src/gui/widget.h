#ifndef WAIDGET_GAAGJFOEQFMO_H
#define WAIDGET_GAAGJFOEQFMO_H

#include "common.h"
#include "widget_theme.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class MainWidget;

class Widget : public sf::Drawable
{
    friend class MainWidget;

public:
    template<typename T, typename... Args>
    static T *create(Args &&...args)
    {
        T *widget = new T{std::forward<Args>(args)...};
        return widget;
    }

    Widget(Widget *parent = nullptr);
    ~Widget();

    const sf::Vector2f &getPosition() const;
    void setPosition(const sf::Vector2f &position);

    const sf::Transform &getTransform() const;

    const sf::Vector2f &getSize() const;
    void setSize(const sf::Vector2f &size);

    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

    Widget *getParent() const;
    void setParent(Widget *widget);
    const std::vector<Widget *> &getChildren() const;
    void removeLastChild();
    void clearChildren();

    WidgetState getState() const;
    bool isFocused() const;

    bool isSelectable() const;
    void setSelectable(bool selectable);

    bool isShow() const;
    void show();
    void hide();

    bool isEnable() const;
    void setEnable(bool enable);

    const WidgetTheme &getWidgetTheme() const;
    void setWidgetTheme(const WidgetTheme &widget_theme, bool children = false);

    sf::Vector2f getChildrenSize() const;
    virtual sf::Vector2f getContentSize() const;

    static const sf::Vector2f &getWindowSize();
    static void setWindowSize(const sf::Vector2f &window_Size);

protected:
    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onMouseMoved(const sf::Vector2f &mouse_position);
    virtual void onMousePressed(const sf::Vector2f &mouse_position);
    virtual void onMouseReleased(const sf::Vector2f &mouse_position);
    virtual void onMouseWheelMoved(int32_t delta);
    virtual void onKeyPressed(const sf::Event::KeyEvent &key);
    virtual void onKeyReleased(const sf::Event::KeyEvent &key);
    virtual void onTextEntered(uint32_t unicode);

    virtual void onDestroyedChild(Widget *widget);
    virtual void onStateChanged(WidgetState state);
    virtual void onResized(const sf::Vector2f &size);
    virtual void onWindowResized(const sf::Vector2f &size);
    virtual void onThemeChanged(const WidgetTheme &widget_theme);

    void setState(WidgetState state);

    Widget *findWidgetAt(const sf::Vector2f &position);

    void drawChildren(sf::RenderTarget &target, const sf::RenderStates &states) const;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

private:
    sf::Vector2f m_position;
    sf::Transform m_transform;
    sf::Vector2f m_size;

    Widget *m_parent;
    std::vector<Widget *> m_children;

    WidgetState m_state;
    bool m_selectable;
    bool m_show;
    bool m_enable;

    WidgetTheme m_widget_theme;

    static sf::Vector2f m_window_size;
};

} // namespace fck::gui

#endif // WAIDGET_H
