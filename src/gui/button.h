#ifndef BUTTON_SLAXZLFHTNLG_H
#define BUTTON_SLAXZLFHTNLG_H

#include "box.h"
#include "label.h"
#include "widget.h"

#include "../fck/sigslot.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Button : public Widget
{
public:
    Button(const sf::String &string = sf::String{}, Widget *parent = nullptr);
    ~Button() = default;

    const sf::String &getString() const;
    void setString(const sf::String &string);

    sf::Vector2f getContentSize() const override;

protected:
    void onStateChanged(WidgetState state) override;
    void onMousePressed(const sf::Vector2f &mouse_position) override;
    void onMouseReleased(const sf::Vector2f &mouse_position) override;
    void onKeyPressed(const sf::Event::KeyEvent &key) override;
    void onKeyReleased(const sf::Event::KeyEvent &key) override;
    void onResized(const sf::Vector2f &size) override;
    void onThemeChanged(const WidgetTheme &widget_theme) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateGeometry();

public:
    Signal<> clicked;

private:
    Box m_background;
    sf::Text m_text;
};

} // namespace fck::gui

#endif // BUTTON_SLAXZLFHTNLG_H
