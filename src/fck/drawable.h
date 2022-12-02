#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "common.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/Rect.hpp"

namespace fck
{

class Entity;

class Drawable
{
    friend class Entity;

public:
    Drawable();
    virtual ~Drawable() = default;

    virtual drawable_type::Type type() const;

    bool isVisible() const;
    void setVisible(bool visible);

    virtual const sf::Color &color() const = 0;
    virtual void setColor(const sf::Color &color) = 0;

    virtual sf::FloatRect localBounds() const = 0;
    virtual sf::FloatRect contentBounds() const = 0;
    virtual sf::Vector2f center() const = 0;

    virtual void draw(sf::RenderTarget &target, const sf::RenderStates &states) = 0;

private:
    bool m_visible;
};

} // namespace fck

#endif // DRAWABLE_H
