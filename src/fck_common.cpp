#include "fck_common.h"

#include <unordered_map>

namespace fck
{

namespace entity_state
{

std::string stateToString(State state)
{
    static std::unordered_map<State, std::string> strings
        = {{State::IDLE, "idle"},
           {State::MOVE, "move"},
           {State::BASE_ATTACK, "base_attack"},
           {State::ULTIMATE_ATTACK, "ultimate_attack"},
           {State::DAMAGED, "damage"},
           {State::DODGE, "dodge"},
           {State::DEATH, "death"}};

    auto strings_found = strings.find(state);
    if (strings_found != strings.end())
        return strings_found->second;
    return "idle";
}

State stateFromString(const std::string &string)
{
    static std::unordered_map<std::string, State> states
        = {{"idle", State::IDLE},
           {"move", State::MOVE},
           {"base_attack", State::BASE_ATTACK},
           {"ultimate_attack", State::ULTIMATE_ATTACK},
           {"damage", State::DAMAGED},
           {"dodge", State::DODGE},
           {"death", State::DEATH}};

    auto states_found = states.find(string);
    if (states_found != states.end())
        return states_found->second;
    return State::IDLE;
}

std::string directionToString(Direction direction)
{
    static std::unordered_map<Direction, std::string> strings
        = {{Direction::LEFT, "left"}, {Direction::RIGHT, "right"}};

    auto strings_found = strings.find(direction);
    if (strings_found != strings.end())
        return strings_found->second;
    return "no_state";
}

Direction directionFromString(const std::string &string)
{
    static std::unordered_map<std::string, Direction> directions
        = {{"left", Direction::LEFT}, {"right", Direction::RIGHT}};

    auto directions_found = directions.find(string);
    if (directions_found != directions.end())
        return directions_found->second;
    return Direction::RIGHT;
}

} // namespace entity_state

namespace component_type
{

std::string toString(Type type)
{
    static std::unordered_map<Type, std::string> strings
        = {{Type::TRANSFORM, "transform"},
           {Type::VELOCITY, "velocity"},
           {Type::SCENE, "scene"},
           {Type::PLAYER, "player"},
           {Type::DRAWABLE, "drawable"},
           {Type::DRAWABLE_ANIMATION, "drawable_animation"},
           {Type::COLLISION, "collision"},
           {Type::STATE, "state"},
           {Type::SOUND, "sound"},
           {Type::SCRIPT, "script"},
           {Type::TARGET, "target"},
           {Type::TARGET_FOLLOW, "target_follow"},
           {Type::LOOK_AROUND, "look_around"},
           {Type::TYPE, "type"},
           {Type::STATS, "stats"},
           {Type::ACTIONS, "actions"},
           {Type::SKILLS, "skills"},
           {Type::DAMAGE, "damage"},
           {Type::MARKER, "marker"},
           {Type::SHADOW, "shadow"}};

    auto strings_found = strings.find(type);
    if (strings_found != strings.end())
        return strings_found->second;
    return "no_type";
}

Type fromString(const std::string &string)
{
    static std::unordered_map<std::string, Type> types
        = {{"transform", Type::TRANSFORM},
           {"velocity", Type::VELOCITY},
           {"scene", Type::SCENE},
           {"player", Type::PLAYER},
           {"drawable", Type::DRAWABLE},
           {"drawable_animation", Type::DRAWABLE_ANIMATION},
           {"collision", Type::COLLISION},
           {"state", Type::STATE},
           {"sound", Type::SOUND},
           {"script", Type::SCRIPT},
           {"target", Type::TARGET},
           {"target_follow", Type::TARGET_FOLLOW},
           {"look_around", Type::LOOK_AROUND},
           {"type", Type::TYPE},
           {"stats", Type::STATS},
           {"actions", Type::ACTIONS},
           {"skills", Type::SKILLS},
           {"damage", Type::DAMAGE},
           {"marker", Type::MARKER},
           {"shadow", Type::SHADOW}};

    auto types_found = types.find(string);
    if (types_found != types.end())
        return types_found->second;
    return Type::NO_TYPE;
}

} // namespace component_type

namespace entity_type
{

std::string toString(Type type)
{
    static std::unordered_map<Type, std::string> strings
        = {{Type::PLAYER, "player"}, {Type::ENEMY, "enemy"}};

    auto strings_found = strings.find(type);
    if (strings_found != strings.end())
        return strings_found->second;
    return "unknow";
}

Type fromString(const std::string &string)
{
    static std::unordered_map<std::string, Type> types
        = {{"player", Type::PLAYER}, {"enemy", Type::ENEMY}};

    auto types_found = types.find(string);
    if (types_found != types.end())
        return types_found->second;
    return Type::NO_TYPE;
}

} // namespace entity_type

namespace tile_material_type
{

std::string toString(Type type)
{
    static std::unordered_map<Type, std::string> strings
        = {{Type::GRASS, "grass"},
           {Type::LIQUID, "liquid"},
           {Type::METAL, "metal"},
           {Type::TILE, "tile"},
           {Type::WOOD, "wood"}};

    auto strings_found = strings.find(type);
    if (strings_found != strings.end())
        return strings_found->second;
    return "unknow";
}

Type fromString(const std::string &string)
{
    static std::unordered_map<std::string, Type> types
        = {{"grass", Type::GRASS},
           {"liquid", Type::LIQUID},
           {"metal", Type::METAL},
           {"tile", Type::TILE},
           {"wood", Type::WOOD}};

    auto types_found = types.find(string);
    if (types_found != types.end())
        return types_found->second;
    return Type::NO_TYPE;
}

} // namespace tile_material_type

} // namespace fck
