#include "common.h"

#include "../fck/utilities.h"

namespace fck::gui
{

void positionText(
    sf::Text &text, const sf::Vector2f &size, const Sides<float> padding, int32_t text_align)
{
    sf::FloatRect text_global_bounds = text.getGlobalBounds();
    sf::FloatRect text_local_bounds = text.getLocalBounds();

    sf::Vector2f text_half_size = text_global_bounds.getSize() / 2.0f;
    sf::Vector2f text_origin = text_half_size;

    sf::Vector2f half_size
        = (size - sf::Vector2f{padding.left + padding.right, padding.top + padding.bottom}) / 2.0f;
    sf::Vector2f text_position = half_size;

    if (text_align & TextAlign::LEFT)
    {
        text_origin.x -= text_half_size.x;
        text_position.x -= half_size.x;
    }

    if (text_align & TextAlign::RIGHT)
    {
        text_origin.x += text_half_size.x;
        text_position.x += half_size.x;
    }

    if (text_align & TextAlign::TOP)
    {
        text_origin.y -= text_half_size.y;
        text_position.y -= half_size.y;
    }

    if (text_align & TextAlign::BOTTOM)
    {
        text_origin.y += text_half_size.y;
        text_position.y += half_size.y;
    }

    text.setOrigin(
        vector2::round(sf::Vector2f{0, text_local_bounds.getPosition().y} + text_origin));
    text.setPosition(vector2::round(sf::Vector2f{padding.left, padding.top} + text_position));
}

} // namespace fck::gui
