#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "common.h"

#include <SFML/Graphics.hpp>

namespace fck
{

class Drawable : public sf::Drawable, public sf::Transformable
{
public:
    Drawable() = default;
    virtual ~Drawable() = default;

    virtual drawable_type::Type type() const = 0;

    virtual const sf::Color &color() const = 0;
    virtual void setColor(const sf::Color &color) = 0;

    virtual sf::FloatRect localBounds() const = 0;
    virtual sf::FloatRect globalBounds() const = 0;
};

} // namespace fck

#endif // DRAWABLE_H
