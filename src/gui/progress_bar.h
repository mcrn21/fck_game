#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "box.h"
#include "widget.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class ProgressBar : public Widget
{
public:
    ProgressBar(Widget *parent = nullptr);
    ~ProgressBar() = default;

    float getProgress() const;
    void setProgress(float progress);

    const sf::String &getString() const;
    void setString(const sf::String &string);

    const sf::Vector2f &getTextOffset() const;
    void setTextOffset(const sf::Vector2f &text_offset);

protected:
    void onResized(const sf::Vector2f &size) override;
    void onThemeChanged(const WidgetTheme &widget_theme) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateGeometry();

private:
    Box m_background;
    Box m_bar;
    sf::Text m_text;
    sf::Vector2f m_text_offset;
    float m_progress;
};

} // namespace fck::gui

#endif // PROGRESSBAR_H
