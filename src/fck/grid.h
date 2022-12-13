#ifndef GRID_HJVHAPZWPEND_H
#define GRID_HJVHAPZWPEND_H

#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace fck
{

template<typename T>
class Grid
{
public:
    Grid();
    ~Grid();

    const sf::Vector2i &getCellSize() const;
    void setCellSize(const sf::Vector2i &cell_size);

    const std::vector<std::vector<T *>> &getGrid() const;
    void setGrid(const std::vector<std::vector<T *>> &grid);

    T *getCell(const sf::Vector2i &coord);
    T *getCellByPosition(const sf::Vector2f &position);

    sf::Vector2i transformPosition(const sf::Vector2f &position) const;

    void resize(const sf::Vector2i &size);
    void clear();

private:
    sf::Vector2i m_cell_size;
    std::vector<std::vector<T *>> m_grid;
};

template<typename T>
Grid<T>::Grid() : m_cell_size{32, 32}
{
}

template<typename T>
Grid<T>::~Grid()
{
    clear();
}

template<typename T>
const sf::Vector2i &Grid<T>::getCellSize() const
{
    return m_cell_size;
}

template<typename T>
void Grid<T>::setCellSize(const sf::Vector2i &cell_size)
{
    m_cell_size = cell_size;
}

template<typename T>
const std::vector<std::vector<T *>> &Grid<T>::getGrid() const
{
    return m_grid;
}

template<typename T>
void Grid<T>::setGrid(const std::vector<std::vector<T *>> &grid)
{
    clear();
    m_grid = grid;
}

template<typename T>
T *Grid<T>::getCell(const sf::Vector2i &coord)
{
    if (coord.x < 0 || coord.y < 0)
        return nullptr;

    if (coord.y >= m_grid.size())
        return nullptr;

    if (coord.x >= m_grid[coord.y].size())
        return nullptr;

    return m_grid[coord.y][coord.x];
}

template<typename T>
T *Grid<T>::getCellByPosition(const sf::Vector2f &position)
{
    if (position.x < 0 || position.y < 0)
        return nullptr;

    int32_t x = position.x / m_cell_size.x;
    int32_t y = position.x / m_cell_size.y;

    return getCell({x, y});
}

template<typename T>
sf::Vector2i Grid<T>::transformPosition(const sf::Vector2f &position) const
{
    return {int32_t(position.x / m_cell_size.x), int32_t(position.y / m_cell_size.y)};
}

template<typename T>
void Grid<T>::resize(const sf::Vector2i &size)
{
    clear();
    for (int32_t i = 0; i < size.y; ++i)
    {
        m_grid.push_back(std::vector<T *>());
        for (int32_t j = 0; j < size.x; ++j)
            m_grid.back().push_back(new T{});
    }
}

template<typename T>
void Grid<T>::clear()
{
    for (auto &row : m_grid)
    {
        for (auto &m_cell : row)
            delete m_cell;
    }
    m_grid.clear();
}

} // namespace fck

#endif // GRID_HJVHAPZWPEND_H
