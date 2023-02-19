#ifndef DRAWABLEPROXY_INWYECDJAZUG_H
#define DRAWABLEPROXY_INWYECDJAZUG_H

#include <SFML/Graphics.hpp>

#include <memory>

namespace fck
{

struct DrawableProxyBase
{
    virtual ~DrawableProxyBase() = default;

    virtual DrawableProxyBase *copy() = 0;

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
    DrawableProxy() : d{nullptr}
    {
    }

    DrawableProxy(T *drawable) : d{drawable}
    {
    }

    DrawableProxy(const DrawableProxy &other)
    {
        d = new T{*other.d};
    }

    DrawableProxy(DrawableProxy &&other)
    {
        d = other.d;
        other.d = nullptr;
    }

    ~DrawableProxy()
    {
        delete d;
    }

    DrawableProxyBase *copy()
    {
        return new DrawableProxy<T>(*this);
    }

    DrawableProxy &operator=(const DrawableProxy &other)
    {
        d = new T{*other.d};
        return *this;
    }

    DrawableProxy &operator=(DrawableProxy &&other)
    {
        d = other.d;
        other.d = nullptr;
        return *this;
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
