#include "b2_dynamic_tree.h"

namespace fck::b2
{

AABB::AABB()
{
}

AABB::AABB(const sf::FloatRect &rect)
{
    lower_bound = rect.getPosition();
    upper_bound.x = lower_bound.x + rect.width;
    upper_bound.y = lower_bound.y + rect.height;
}

sf::Vector2f AABB::center() const
{
    return 0.5f * (lower_bound + upper_bound);
}

sf::Vector2f AABB::extents() const
{
    return 0.5f * (upper_bound - lower_bound);
}

float AABB::perimeter() const
{
    float wx = upper_bound.x - lower_bound.x;
    float wy = upper_bound.y - lower_bound.y;
    return 2.0f * (wx + wy);
}

void AABB::combine(const AABB &aabb)
{
    lower_bound = sf::Vector2f(
        std::min(lower_bound.x, aabb.lower_bound.x), std::min(lower_bound.y, aabb.lower_bound.y));
    upper_bound = sf::Vector2f(
        std::max(upper_bound.x, aabb.upper_bound.x), std::max(upper_bound.y, aabb.upper_bound.y));
}

void AABB::combine(const AABB &aabb1, const AABB &aabb2)
{
    lower_bound = sf::Vector2f(
        std::min(aabb1.lower_bound.x, aabb2.lower_bound.x),
        std::min(aabb1.lower_bound.y, aabb2.lower_bound.y));
    upper_bound = sf::Vector2f(
        std::max(aabb1.upper_bound.x, aabb2.upper_bound.x),
        std::max(aabb1.upper_bound.y, aabb2.upper_bound.y));
}

bool AABB::contains(const AABB &aabb) const
{
    bool result = true;
    result = result && lower_bound.x <= aabb.lower_bound.x;
    result = result && lower_bound.y <= aabb.lower_bound.y;
    result = result && aabb.upper_bound.x <= upper_bound.x;
    result = result && aabb.upper_bound.y <= upper_bound.y;
    return result;
}

} // namespace fck::b2
