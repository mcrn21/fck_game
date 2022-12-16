#ifndef DRAWABLEPROXY_INWYECDJAZUG_H
#define DRAWABLEPROXY_INWYECDJAZUG_H

#include <SFML/Graphics.hpp>

#include <memory>

namespace fck
{

struct DrawableProxyBase
{
    virtual ~DrawableProxyBase() = default;

    virtual sf::Transformable *toTransformable() const = 0;
    virtual sf::Drawable *toDrawable() const = 0;

    virtual const sf::Color &getColor() const = 0;
    virtual void setColor(const sf::Color &color) = 0;

    virtual sf::FloatRect getLocalBounds() const = 0;
    virtual sf::FloatRect getGlobalBounds() const = 0;

    virtual void *data() = 0;
};

template<typename T>
struct DrawableProxy : DrawableProxyBase
{
    DrawableProxy(T *drawable) : d{drawable}
    {
    }

    ~DrawableProxy()
    {
        delete d;
    }

    sf::Transformable *toTransformable() const
    {
        return d;
    }

    sf::Drawable *toDrawable() const
    {
        return d;
    }

    const sf::Color &getColor() const
    {
        return d->getColor();
    }

    void setColor(const sf::Color &color)
    {
        d->setColor(color);
    }

    sf::FloatRect getLocalBounds() const
    {
        return d->getLocalBounds();
    }

    sf::FloatRect getGlobalBounds() const
    {
        return d->getGlobalBounds();
    }

    void *data()
    {
        return d;
    }

    T *d;
};

} // namespace fck

#endif // DRAWABLEPROXY_H
