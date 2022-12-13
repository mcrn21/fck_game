#ifndef DRAWABLE_FIVYUHNDIKNR_H
#define DRAWABLE_FIVYUHNDIKNR_H

#include "common.h"

#include <SFML/Graphics.hpp>

namespace fck
{

class Drawable : public sf::Drawable, public sf::Transformable
{
public:
    Drawable() = default;
    virtual ~Drawable() = default;

    virtual drawable_type::Type getType() const = 0;

    virtual const sf::Color &getColor() const = 0;
    virtual void setColor(const sf::Color &color) = 0;

    virtual sf::FloatRect getLocalBounds() const = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;
};

} // namespace fck

#endif // DRAWABLE_FIVYUHNDIKNR_H
