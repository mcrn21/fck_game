#ifndef COMMON_SEIHTSWCWDCI_H
#define COMMON_SEIHTSWCWDCI_H

#include <SFML/Graphics.hpp>

namespace fck::gui
{

enum WidgetState
{
    DEFAULT = 1,
    HOVERED = 2,
    PRESSED = 4,
    FOCUSED = 8
};

enum TextAlign
{
    LEFT = 1,
    TOP = 2,
    RIGHT = 4,
    BOTTOM = 8,
    H_CENTER = LEFT | RIGHT,
    V_CENTER = TOP | BOTTOM,
    CENTER = H_CENTER | V_CENTER
};

enum Direction
{
    LEFT_TO_RIGHT,
    RIGHT_TO_LEFT
};

enum DialogFlags
{
    TITLE = 1,
    CLOSE = 2
};

template<typename T>
struct Sides
{
    Sides() : left{0}, top{0}, right{0}, bottom{0}
    {
    }
    Sides(T v) : left{v}, top{v}, right{v}, bottom{v}
    {
    }

    Sides(T left, T top, T right, T bottom) : left{left}, top{top}, right{right}, bottom{bottom}
    {
    }

    sf::Vector2<T> getLowerPoint() const
    {
        return {left, top};
    }

    sf::Vector2<T> getUpperPoint() const
    {
        return {right, bottom};
    }

    T left;
    T top;
    T right;
    T bottom;
};

void positionText(
    sf::Text &text, const sf::Vector2f &size, const Sides<float> padding, int32_t text_align);

} // namespace fck::gui

#endif // COMMON_SEIHTSWCWDCI_H
