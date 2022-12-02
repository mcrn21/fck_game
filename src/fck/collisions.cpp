#include "collisions.h"
#include "utilities.h"

#include <algorithm>
#include <cmath>

namespace fck::collisions
{

const float EPSILON = 1e-8;

AABB::AABB(const sf::FloatRect &rect)
{
    position = {rect.left + rect.width / 2, rect.top + rect.height / 2};
    half = {rect.width / 2, rect.height / 2};
}

std::optional<Hit> AABB::intersectSegment(const sf::Vector2f &pos, const sf::Vector2f &delta) const
{
    sf::Vector2f min = position - half;
    sf::Vector2f max = position + half;

    float last_entry = -std::numeric_limits<float>::max();
    float first_exit = std::numeric_limits<float>::max();

    if (delta.x != 0)
    {
        float t1 = (min.x - pos.x) / delta.x;
        float t2 = (max.x - pos.x) / delta.x;

        last_entry = std::max(last_entry, std::min(t1, t2));
        first_exit = std::min(first_exit, std::max(t1, t2));
    }
    else if (pos.x <= min.x || pos.x >= max.x)
    {
        return std::nullopt;
    }

    if (delta.y != 0)
    {
        float t1 = (min.y - pos.y) / delta.y;
        float t2 = (max.y - pos.y) / delta.y;

        last_entry = std::max(last_entry, std::min(t1, t2));
        first_exit = std::min(first_exit, std::max(t1, t2));
    }
    else if (pos.y <= min.y || pos.y >= max.y)
    {
        return std::nullopt;
    }

    Hit hit;

    if (first_exit > last_entry && first_exit > 0 && last_entry < 1)
    {
        hit.position.x = pos.x + delta.x * last_entry;
        hit.position.y = pos.y + delta.y * last_entry;

        hit.time = last_entry;

        float dx = hit.position.x - position.x;
        float dy = hit.position.y - position.y;
        float px = half.x - std::abs(dx);
        float py = half.y - std::abs(dy);

        if (px < py)
        {
            hit.normal.x = (dx > 0) - (dx < 0);
        }
        else
        {
            hit.normal.y = (dy > 0) - (dy < 0);
        }
    }

    return hit;
}

std::optional<Hit> AABB::intersectAABB(const AABB &other) const
{
    float dx = other.position.x - position.x;
    float px = (other.half.x + half.x) - std::abs(dx);
    if (px <= 0.0f)
        return std::nullopt;

    float dy = other.position.y - position.y;
    float py = (other.half.y + half.y) - std::abs(dy);
    if (py <= 0.0f)
        return std::nullopt;

    Hit hit;

    if (px < py)
    {
        int8_t sx = number::sign(dx);
        hit.delta.x = px * sx;
        hit.normal.x = sx;
        hit.position.x = position.x + (half.x * sx);
        hit.position.y = other.position.y;
    }
    else
    {
        int8_t sy = number::sign(dy);
        hit.delta.y = py * sy;
        hit.normal.y = sy;
        hit.position.x = other.position.x;
        hit.position.y = position.y + (half.y * sy);
    }

    return hit;
}

std::optional<Hit> AABB::sweepAABB(const AABB &other, const sf::Vector2f &delta) const
{
    AABB sum_other = other;
    sum_other.half = half + other.half;

    auto hit = sum_other.intersectSegment(position, delta);

    return hit;
}

std::optional<Hit> AABB::sweepInto(
    const std::list<AABB> &aabb_list, const sf::Vector2f &delta) const
{
    std::optional<Hit> hit;
    float time = 1;

    for (const AABB &aabb : aabb_list)
    {
        auto h = sweepAABB(aabb, delta);
        if (h)
        {
            if (h->time < time)
            {
                time = h->time;
                hit = h;
            }
        }
    }

    return hit;
}

} // namespace fck::collisions
