#ifndef FCK_COMMON_H
#define FCK_COMMON_H

#include "fck/common.h"

#include <cstdint>

namespace fck
{

const int32_t Z_ORDER = 1000000;

namespace gui
{

const int32_t FONT_SIZE = 28;
const int32_t HP_HUD_SIZE = 26;

} // namespace gui

namespace event_type
{

enum Type
{
    EXIT_GAME = 1000,
    NEW_GAME,
    RETURN_TO_GAME,
    RETURN_TO_MAIN_MENU,
};

}

namespace keyboard_action
{

enum Action
{
    TOGGLE_RENDER_DEBUG = 10000,
    TOOGLE_COLLISION_DEBUG = 10001,

    BACK = 12000,

    PLAYER_MOVE_LEFT = 14001,
    PLAYER_MOVE_UP = 14002,
    PLAYER_MOVE_RIGHT = 14003,
    PLAYER_MOVE_DOWN = 14004,

    CHANGE_TARGET = 15000,

    PLAYER_SKILL_1 = 16000,
    PLAYER_SKILL_2 = 16001,
    PLAYER_SKILL_3 = 16002,
    PLAYER_SKILL_4 = 16003
};

}

namespace game_state
{
enum State
{
    NO_STATE,
    FIRST_LOADING,
    LOADING,
    MAIN_MENU,
    LEVEL,
    LEVEL_MENU
};
}

namespace entity_state
{

enum State
{
    IDLE = 1,
    MOVE = 2,
    BASE_ATTACK = 4,
    ULTIMATE_ATTACK = 8,
    DAMAGED = 16,
    DODGE = 32,
    DEATH = 64,

    ATTACK = BASE_ATTACK | ULTIMATE_ATTACK,
    NOT_AVALIBLE = ATTACK | DAMAGED | DODGE | DEATH
};

std::string stateToString(State state);
State stateFromString(const std::string &string);

enum Direction
{
    LEFT = -1,
    RIGHT = 1
};

std::string directionToString(Direction direction);
Direction directionFromString(const std::string &string);

} // namespace entity_state

namespace component_type
{

enum Type
{
    NO_TYPE,
    TRANSFORM,
    VELOCITY,
    SCENE,
    PLAYER,
    DRAWABLE,
    DRAWABLE_ANIMATION,
    COLLISION,
    STATE,
    SOUND,
    SCRIPT,
    TARGET,
    TARGET_FOLLOW,
    LOOK_AROUND,
    TYPE,
    STATS,
    ACTIONS,
    SKILLS,
    DAMAGE,
    MARKER,
    SHADOW
};

std::string toString(Type type);
Type fromString(const std::string &string);
} // namespace component_type

namespace entity_type
{

enum Type
{
    NO_TYPE,
    PLAYER,
    ENEMY
};

std::string toString(Type type);
Type fromString(const std::string &string);
} // namespace entity_type

} // namespace fck

#endif // FCK_COMMON_H
