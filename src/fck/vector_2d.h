#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <SFML/System/Vector2.hpp>

#include <stdexcept>
#include <vector>

namespace fck
{

template<typename T>
class Vector2D
{
public:
    Vector2D()
    {
    }
    Vector2D(const sf::Vector2i &size, T value = T()) : m_size{size}
    {
        m_data.resize(m_size.x * m_size.y, value);
    }
    ~Vector2D() = default;

    sf::Vector2i transformIndex(int32_t index) const
    {
        return {index % m_size.x, index / m_size.x};
    }

    T &data(const sf::Vector2i &coords)
    {
        int32_t index = coords.y * m_size.x + coords.x;
        if (index > m_data.size())
            throw std::out_of_range{"Wrong [x, y] coordinate"};
        return m_data[index];
    }

    const T &data(const sf::Vector2i &coords) const
    {
        int32_t index = coords.y * m_size.x + coords.x;
        if (index > m_data.size())
            throw std::out_of_range{"Wrong [x, y] coordinate"};
        return m_data.at(index);
    }

    const T &at(int32_t index) const
    {
        return m_data.at(index);
    }

    T &operator[](int32_t index)
    {
        return m_data[index];
    }

    const sf::Vector2i &size2D() const
    {
        return m_size;
    }

    std::size_t size() const
    {
        return m_data.size();
    }

    void resize(const sf::Vector2i &size, T value = T())
    {
        m_size = size;
        m_data.resize(m_size.x * m_size.y, value);
    }

    void clear()
    {
        m_size = {0, 0};
        m_data.clear();
    }

private:
    sf::Vector2i m_size;
    std::vector<T> m_data;
};

} // namespace fck

#endif // VECTOR_2D_H
