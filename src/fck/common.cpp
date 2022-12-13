#include "common.h"

#include <unordered_map>

namespace fck
{

namespace drawable_type
{

std::string toString(Type type)
{
    static std::unordered_map<Type, std::string> strings
        = {{Type::SPRITE, "sprite"}, {Type::TILE_MAP, "tile_map"}};

    auto strings_found = strings.find(type);
    if (strings_found != strings.end())
        return strings_found->second;
    return "no_type";
}

Type fromString(const std::string &string)
{
    static std::unordered_map<std::string, Type> types
        = {{"sprite", Type::SPRITE}, {"tile_map", Type::TILE_MAP}};

    auto types_found = types.find(string);
    if (types_found != types.end())
        return types_found->second;
    return Type::NO_TYPE;
}

} // namespace drawable_type

} // namespace fck
