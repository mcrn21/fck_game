#ifndef GRID_HJVHAPZWPEND_H
#define GRID_HJVHAPZWPEND_H

#include "vector_2d.h"

#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace fck
{

template<typename T>
class Grid : public Vector2D<T>
{
public:
    Grid() : Vector2D<T>{}
    {
    }

    Grid(const sf::Vector2i &tile_size, const sf::Vector2i &grid_size, T init = T{})
        : Vector2D<T>{grid_size, init}, m_tile_size{tile_size}
    {
    }

    const sf::Vector2i &getTileSize() const
    {
        return m_tile_size;
    }

    void setTileSize(const sf::Vector2i &tile_size)
    {
        m_tile_size = tile_size;
    }

    T &getDataByPosition(const sf::Vector2f &position)
    {
        sf::Vector2i coord
            = {int32_t(position.x / m_tile_size.x), int32_t(position.y / m_tile_size.y)};
        return Vector2D<T>::getData(coord);
    }

    const T &getDataByPosition(const sf::Vector2f &position) const
    {
        sf::Vector2i coord
            = {int32_t(position.x / m_tile_size.x), int32_t(position.y / m_tile_size.y)};
        return Vector2D<T>::getData(coord);
    }

    sf::Vector2i transformPosition(const sf::Vector2f &position) const
    {
        return {int32_t(position.x / m_tile_size.x), int32_t(position.y / m_tile_size.y)};
    }

private:
    sf::Vector2i m_tile_size;
};

} // namespace fck

#endif // GRID_HJVHAPZWPEND_H
