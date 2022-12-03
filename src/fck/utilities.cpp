#include "utilities.h"

namespace fck
{

namespace string
{

void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

void rtrim(std::string &s)
{
    s.erase(
        std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); })
            .base(),
        s.end());
}

void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

std::string ltrimCopy(std::string s)
{
    ltrim(s);
    return s;
}

std::string rtrimCopy(std::string s)
{
    rtrim(s);
    return s;
}

std::string trimCopy(std::string s)
{
    trim(s);
    return s;
}

std::vector<std::string> split(const std::string &s, char seperator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        output.push_back(substring);
        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
    return output;
}

void toLower(std::string &string)
{
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
}

} // namespace string

namespace vector2
{

sf::Vector2i tomlArrayToVector2i(toml::array *array)
{
    if (array->size() < 2)
        return sf::Vector2i{};

    sf::Vector2i vector;

    vector.x = array->at(0).as_integer()->get();
    vector.y = array->at(1).as_integer()->get();

    return vector;
}

sf::Vector2f tomlArrayToVector2f(toml::array *array)
{
    if (array->size() < 2)
        return sf::Vector2f{};

    sf::Vector2f vector;

    vector.x = array->at(0).as_floating_point()->get();
    vector.y = array->at(1).as_floating_point()->get();

    return vector;
}

} // namespace vector2

namespace rect
{

sf::IntRect tomlArrayToIntRect(toml::array *array)
{
    if (array->size() < 4)
        return sf::IntRect{};

    sf::IntRect rect;

    rect.left = array->at(0).as_integer()->get();
    rect.top = array->at(1).as_integer()->get();
    rect.width = array->at(2).as_integer()->get();
    rect.height = array->at(3).as_integer()->get();

    return rect;
}

sf::FloatRect tomlArrayToFloatRect(toml::array *array)
{
    if (array->size() < 4)
        return sf::FloatRect{};

    sf::FloatRect rect;

    rect.left = array->at(0).as_floating_point()->get();
    rect.top = array->at(1).as_floating_point()->get();
    rect.width = array->at(2).as_floating_point()->get();
    rect.height = array->at(3).as_floating_point()->get();

    return rect;
}

} // namespace rect

namespace pair
{

std::pair<int32_t, int32_t> tomlArrayToIntPair(toml::array *array)
{
    if (array->size() < 2)
        return {0, 0};

    std::pair<int32_t, int32_t> pair;

    pair.first = array->at(0).as_integer()->get();
    pair.second = array->at(1).as_integer()->get();

    return pair;
}

std::pair<float, float> tomlArrayToFloatPair(toml::array *array)
{
    if (array->size() < 2)
        return {0, 0};

    std::pair<float, float> pair;

    pair.first = array->at(0).as_floating_point()->get();
    pair.second = array->at(1).as_floating_point()->get();

    return pair;
}

std::pair<double, double> tomlArrayToDoublePair(toml::array *array)
{
    if (array->size() < 2)
        return {0, 0};

    std::pair<double, double> pair;

    pair.first = array->at(0).as_floating_point()->get();
    pair.second = array->at(1).as_floating_point()->get();

    return pair;
}

} // namespace pair

namespace vector
{

std::vector<std::string> tomlArrayToStringVector(toml::array *array)
{
    std::vector<std::string> vec;
    for (auto &v : *array)
        vec.push_back(v.as_string()->get());
    return vec;
}

} // namespace vector

} // namespace fck
