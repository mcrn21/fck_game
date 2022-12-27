#ifndef DIALOG_ZPLESKPYIQYQ_H
#define DIALOG_ZPLESKPYIQYQ_H

#include "box.h"
#include "button.h"
#include "widget.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Dialog : public Widget
{
public:
    Dialog(const sf::String &title_string = sf::String{}, Widget *parent = nullptr);
    ~Dialog() = default;

    const sf::String &getTitleString() const;
    void setTitleString(const sf::String &string);

    int32_t getDialogFlags() const;
    void setDialoFlags(int32_t dialog_flags);

protected:
    void onMouseMoved(const sf::Vector2f &mouse_position) override;
    void onMousePressed(const sf::Vector2f &mouse_position) override;
    void onMouseReleased(const sf::Vector2f &mouse_position) override;

    void onResized(const sf::Vector2f &size) override;
    void onThemeChanged(const WidgetTheme &widget_theme) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateGeometry();

private:
    int32_t m_dialog_flags;

    bool m_drag;
    sf::Vector2f m_mouse_position;

    Box m_background;
    Box m_close_button;
    WidgetState m_close_button_state;

    sf::Text m_title_text;
    sf::Vector2f m_title_text_size;
};

class QuestionDialog : public Dialog
{
public:
    QuestionDialog(
        const sf::String &title_string = sf::String{},
        const sf::String &string = sf::String{},
        const sf::String &yes_string = sf::String{},
        const sf::String &no_string = sf::String{},
        Widget *parent = nullptr);
    ~QuestionDialog() = default;

    Label *getBodyLabel() const;
    Button *getYesButton() const;
    Button *getNoButton() const;

protected:
    void onResized(const sf::Vector2f &size) override;

private:
    void updateGeometry();

private:
    Label *m_body_label;
    Button *m_yes_button;
    Button *m_no_button;
};

} // namespace fck::gui

#endif // DIALOG_ZPLESKPYIQYQ_H
