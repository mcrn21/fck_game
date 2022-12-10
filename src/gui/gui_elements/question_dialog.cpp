#include "question_dialog.h"

#include "../gui_style.h"

#include "../../gui_base.h"

#include "../../fck/clipping.h"
#include "../../fck/utilities.h"

namespace fck::gui
{

QuestionDialog::QuestionDialog() : Frame{}
{
}

QuestionDialog::QuestionDialog(
    const sf::String &text,
    const sf::String &ok_text,
    const sf::String &cancel_text,
    sf::Texture &texture,
    const sf::IntRect &frame_texture_rect)
    : Frame{texture, frame_texture_rect}
{
    m_ok_button = new Button{};
    m_ok_button->setParent(this);

    m_cancel_button = new Button{};
    m_cancel_button->setParent(this);
}

sf::Text &QuestionDialog::text()
{
    return m_text;
}

Button *QuestionDialog::okButton() const
{
    return m_ok_button;
}

Button *QuestionDialog::cancelButton() const
{
    return m_cancel_button;
}

void QuestionDialog::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    Frame::draw(target, states);

    {
        sf::RenderStates new_state = states;
        new_state.texture = getTexture();
        new_state.transform *= getParentsTransform();

        Clipping clipping(
            target, new_state, {2.0f, 2.0f}, getLocalBounds().getSize() - sf::Vector2f{4.0f, 5.0f});

        target.draw(m_text, new_state);
    }
}

} // namespace fck::gui
