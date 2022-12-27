#include "dialog.h"

#include "../fck/clipping.h"
#include "../fck/resource_cache.h"

namespace fck::gui
{

Dialog::Dialog(const sf::String &title_string, Widget *parent)
    : Widget{parent},
      m_dialog_flags{DialogFlags::TITLE | DialogFlags::CLOSE},
      m_drag{false},
      m_close_button_state{WidgetState::DEFAULT}
{
    setSelectable(false);
    hide();

    m_title_text.setString(title_string);
    setWidgetTheme(WidgetTheme::get<Dialog>());
}

const sf::String &Dialog::getTitleString() const
{
    return m_title_text.getString();
}

void Dialog::setTitleString(const sf::String &string)
{
    m_title_text.setString(string);
    updateGeometry();
}

int32_t Dialog::getDialogFlags() const
{
    return m_dialog_flags;
}

void Dialog::setDialoFlags(int32_t dialog_flags)
{
    m_dialog_flags = dialog_flags;
    updateGeometry();
}

void Dialog::onMouseMoved(const sf::Vector2f &mouse_position)
{
    if (m_drag)
    {
        sf::Vector2f delta = mouse_position - m_mouse_position;
        m_mouse_position = mouse_position;
        setPosition(getPosition() + delta);
        return;
    }

    if (m_dialog_flags & DialogFlags::CLOSE)
    {
        const WidgetTheme &widget_theme = getWidgetTheme();
        sf::Vector2f mouse_local_position = mouse_position - getGlobalBounds().getPosition();
        if (m_close_button_state != WidgetState::HOVERED
            && m_close_button.containsPoint(mouse_local_position))
        {
            m_close_button_state = WidgetState::HOVERED;
            m_close_button.setTextureRect(
                widget_theme.close.texture_rects.at(m_close_button_state));
        }
        else if (
            m_close_button_state != WidgetState::DEFAULT
            && !m_close_button.containsPoint(mouse_local_position))
        {
            m_close_button_state = WidgetState::DEFAULT;
            m_close_button.setTextureRect(
                widget_theme.close.texture_rects.at(m_close_button_state));
        }
    }
}

void Dialog::onMousePressed(const sf::Vector2f &mouse_position)
{
    sf::Vector2f mouse_local_position = mouse_position - getGlobalBounds().getPosition();
    const WidgetTheme &widget_theme = getWidgetTheme();

    if ((m_dialog_flags & DialogFlags::CLOSE) && m_close_button.containsPoint(mouse_local_position))
    {
        m_close_button_state = WidgetState::PRESSED;
        m_close_button.setTextureRect(widget_theme.close.texture_rects.at(m_close_button_state));
        return;
    }

    if ((m_dialog_flags & DialogFlags::TITLE) && mouse_local_position.y < widget_theme.padding.top)
    {
        m_drag = true;
        m_mouse_position = mouse_position;
        return;
    }
}

void Dialog::onMouseReleased(const sf::Vector2f &mouse_position)
{
    const WidgetTheme &widget_theme = getWidgetTheme();
    sf::Vector2f mouse_local_position = mouse_position - getGlobalBounds().getPosition();

    if ((m_dialog_flags & DialogFlags::CLOSE) && m_close_button_state == WidgetState::PRESSED)
    {
        m_close_button_state = m_close_button.containsPoint(mouse_local_position)
            ? WidgetState::HOVERED
            : WidgetState::DEFAULT;
        m_close_button.setTextureRect(widget_theme.close.texture_rects.at(m_close_button_state));
        hide();
        return;
    }

    if (m_drag)
    {
        m_drag = false;
        return;
    }
}

void Dialog::onResized(const sf::Vector2f &size)
{
    updateGeometry();
}

void Dialog::onThemeChanged(const WidgetTheme &widget_theme)
{
    m_background.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_background.setBorderSize(widget_theme.background.border_size);
    m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
    m_background.setBorderTextureSize(widget_theme.background.texture_border_size);

    m_close_button.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_close_button.setBorderSize(widget_theme.close.border_size);
    m_close_button.setTextureRect(widget_theme.close.texture_rects.at(getState()));
    m_close_button.setBorderTextureSize(widget_theme.close.texture_border_size);

    m_title_text.setFont(*ResourceCache::get<sf::Font>(widget_theme.font));
    m_title_text.setStyle(widget_theme.text.style);
    m_title_text.setCharacterSize(widget_theme.text.character_size);
    m_title_text.setFillColor(widget_theme.text.fill_colors.at(getState()));
    m_title_text.setOutlineThickness(widget_theme.text.outline_thickness);
    m_title_text.setOutlineColor(widget_theme.text.outline_colors.at(getState()));

    updateGeometry();
}

void Dialog::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= getTransform();

    target.draw(m_background, new_states);

    if (m_dialog_flags & DialogFlags::CLOSE)
        target.draw(m_close_button, new_states);

    if (m_dialog_flags & DialogFlags::TITLE)
    {
        const WidgetTheme &widget_theme = getWidgetTheme();
        {
            Clipping clipping(
                target,
                new_states,
                {widget_theme.header.padding.left, widget_theme.header.padding.top},
                m_title_text_size
                    - sf::Vector2f{
                        m_close_button.getSize().x + widget_theme.header.padding.right * 2, 0});

            target.draw(m_title_text, new_states);
        }
    }

    drawChildren(target, new_states);
}

void Dialog::updateGeometry()
{
    m_background.setSize(getSize());

    const WidgetTheme &widget_theme = getWidgetTheme();

    float close_button_height = widget_theme.padding.top - widget_theme.header.padding.top
        - widget_theme.header.padding.bottom;
    m_close_button.setSize({close_button_height, close_button_height});

    m_close_button.setPoisition(
        {getSize().x - close_button_height - widget_theme.header.padding.right,
         widget_theme.header.padding.top});

    m_title_text_size = {getSize().x - widget_theme.header.padding.right, widget_theme.padding.top};

    positionText(m_title_text, m_title_text_size, 0.0f, widget_theme.text.align);
}

QuestionDialog::QuestionDialog(
    const sf::String &title_string,
    const sf::String &string,
    const sf::String &yes_string,
    const sf::String &no_string,
    Widget *parent)
    : Dialog{title_string, parent},
      m_body_label{new Label{string, this}},
      m_yes_button{new Button{yes_string, this}},
      m_no_button{new Button{no_string, this}}
{
    updateGeometry();
}

Label *QuestionDialog::getBodyLabel() const
{
    return m_body_label;
}

Button *QuestionDialog::getYesButton() const
{
    return m_yes_button;
}

Button *QuestionDialog::getNoButton() const
{
    return m_no_button;
}

void QuestionDialog::onResized(const sf::Vector2f &size)
{
    Dialog::onResized(size);
    updateGeometry();
}

void QuestionDialog::updateGeometry()
{
    const WidgetTheme &widget_theme = getWidgetTheme();

    sf::Vector2f upper_point
        = getSize() - widget_theme.padding.getLowerPoint() - widget_theme.padding.getUpperPoint();

    m_no_button->setSize(m_no_button->getContentSize());
    m_no_button->setPosition(
        {upper_point.x - m_no_button->getSize().x, upper_point.y - m_no_button->getSize().y});

    m_yes_button->setSize(m_yes_button->getContentSize());
    m_yes_button->setPosition(
        {upper_point.x - m_no_button->getSize().x - widget_theme.padding.right
             - m_yes_button->getSize().x,
         upper_point.y - m_yes_button->getSize().y});

    float buttons_height = std::max(m_yes_button->getSize().y, m_no_button->getSize().y);

    m_body_label->setSize(
        {getSize().x - widget_theme.padding.left - widget_theme.padding.right,
         getSize().y - widget_theme.padding.top - widget_theme.padding.bottom - buttons_height});
}

} // namespace fck::gui
