#ifndef UTILITIES_H
#define UTILITIES_H

#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

#include "toml++/toml.h"

#include <algorithm>
#include <cmath>
#include <exception>
#include <string>

#define fck_assert(condition, failMessage) \
    if (!(condition)) \
        throw ::fck::Exception{ \
            std::string{"Failed at: "} + std::string{__FILE__} + std::string{":"} \
            + std::to_string(__LINE__) + " " + std::string{failMessage}};

namespace fck
{

class Exception : public std::exception
{
public:
    Exception(const std::string &message) : m_message{message}
    {
    }
    ~Exception() = default;

    const char *what() const noexcept
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

namespace string
{

// Strings
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

std::string ltrimCopy(std::string s);
std::string rtrimCopy(std::string s);
std::string trimCopy(std::string s);

std::vector<std::string> split(const std::string &s, char seperator);

void toLower(std::string &string);

} // namespace string

namespace vector2
{

template<typename T>
float distance(const sf::Vector2<T> &point1, const sf::Vector2<T> &point2)
{
    float x_dist = std::abs(point2.x - point1.x);
    float y_dist = std::abs(point2.y - point1.y);
    return std::sqrt(x_dist * x_dist + y_dist * y_dist);
}

template<typename T>
float angleTo(const sf::Vector2<T> &point1, const sf::Vector2<T> &point2)
{
    return std::atan2(point1.y - point2.y, point1.x - point2.x);
}

template<typename T>
bool isValid(const sf::Vector2<T> &vector)
{
    return vector.x != 0 || vector.y != 0;
}

sf::Vector2i tomlArrayToVector2i(toml::array *array);
sf::Vector2f tomlArrayToVector2f(toml::array *array);

} // namespace vector2

namespace rect
{

template<typename T>
sf::Rect<T> extends(const sf::Rect<T> &rect, const sf::Vector2<T> e)
{
    sf::Rect<T> new_rect;
    new_rect.left = rect.left - e.x;
    new_rect.top = rect.top - e.y;
    new_rect.width = rect.width + e.x * 2;
    new_rect.height = rect.height + e.y * 2;
    return new_rect;
}

template<typename T>
sf::Rect<T> setCenter(const sf::Rect<T> &rect, const sf::Vector2<T> center)
{
    sf::Rect<T> new_rect;
    new_rect.left = center.x - rect.width / 2;
    new_rect.top = center.y - rect.height / 2;
    return new_rect;
}

template<typename T>
sf::Vector2<T> center(const sf::Rect<T> &rect)
{
    return sf::Vector2<T>{rect.left + rect.width / 2, rect.top + rect.height / 2};
}

sf::IntRect tomlArrayToIntRect(::toml::array *array);
sf::FloatRect tomlArrayToFloatRect(::toml::array *array);

} // namespace rect

namespace number
{

template<typename T>
inline int8_t sign(const T &value)
{
    return (T(0) < value) - (value < T(0));
}

} // namespace number

} // namespace fck

#endif // UTILITIES_H
