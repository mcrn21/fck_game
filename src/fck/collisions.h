#ifndef COLLISIONS_DJNNLQSSJOWH_H
#define COLLISIONS_DJNNLQSSJOWH_H

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

#include <cmath>
#include <cstdint>
#include <list>

namespace fck::collisions
{

struct Hit
{
    sf::Vector2f position;
    sf::Vector2f delta;
    sf::Vector2f normal;
    float time = 0;
};

struct Sweep
{
    std::optional<Hit> hit;
    sf::Vector2f position;
    float time = 0;
};

struct AABB
{
    AABB() = default;
    AABB(const sf::FloatRect &rect);

    std::optional<Hit> intersectSegment(const sf::Vector2f &pos, const sf::Vector2f &delta) const;
    std::optional<Hit> intersectAABB(const AABB &other) const;
    std::optional<Hit> sweepAABB(const AABB &other, const sf::Vector2f &delta) const;
    std::optional<Hit> sweepInto(const std::list<AABB> &aabb_list, const sf::Vector2f &delta) const;

    sf::Vector2f position;
    sf::Vector2f half;
};

} // namespace fck::collisions

#endif // COLLISIONS_DJNNLQSSJOWH_H
