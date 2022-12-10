#ifndef QUESTIONDIALOG_RDFFRCEXCGUA_H
#define QUESTIONDIALOG_RDFFRCEXCGUA_H

#include "button.h"
#include "frame.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class QuestionDialog : public Frame
{
public:
    QuestionDialog();
    QuestionDialog(
        const sf::String &text,
        const sf::String &ok_text,
        const sf::String &cancel_text,
        sf::Texture &texture,
        const sf::IntRect &frame_texture_rect);
    ~QuestionDialog() = default;

    sf::Text &text();
    Button *okButton() const;
    Button *cancelButton() const;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

private:
    sf::Text m_text;
    Button *m_ok_button;
    Button *m_cancel_button;
};

} // namespace fck::gui

#endif // QUESTIONDIALOG_RDFFRCEXCGUA_H
