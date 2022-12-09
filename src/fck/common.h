#ifndef COMMON_OZWWVWGYBOZG_H
#define COMMON_OZWWVWGYBOZG_H

#include <cstdint>
#include <string>

namespace fck
{

namespace event_type
{

const int32_t TASK = 10300;
const int32_t TASKS = 10301;
const int32_t TASK_SEQUENCE = 10302;

} // namespace event_type

namespace drawable_type
{
enum Type
{
    NO_TYPE,
    SPRITE,
    TILE_MAP
};

std::string toString(Type type);
Type fromString(const std::string &string);
} // namespace drawable_type

namespace drawable_animation_type
{
enum Type
{
    NO_TYPE,
    SPRITE
};

std::string toString(Type type);
Type fromString(const std::string &string);
} // namespace drawable_animation_type

} // namespace fck

#endif // COMMON_OZWWVWGYBOZG_H
