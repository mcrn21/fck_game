#include "bindings.h"

#include "../fck_common.h"

namespace fck
{

void bindFckCommonToLua(sol::state &sol_state)
{
    sol::table fck_namespace = sol_state["fck"].get_or_create<sol::table>();

    fck_namespace.new_enum(
        "EntityState",
        "NO_STATE",
        entity_state::NO_STATE,
        "IDLE",
        entity_state::IDLE,
        "MOVE",
        entity_state::MOVE,
        "ATTACK",
        entity_state::ATTACK,
        "DAMAGED",
        entity_state::DAMAGED,
        "DODGE",
        entity_state::DODGE,
        "DEATH",
        entity_state::DEATH,
        "NOT_AVALIBLE",
        entity_state::NOT_AVALIBLE);

    fck_namespace["entityStateToString"] = &entity_state::stateToString;
    fck_namespace["entityStateFromString"] = &entity_state::stateFromString;

    fck_namespace.new_enum(
        "EntityDirection",
        "NO_DIRECTION",
        entity_state::NO_DIRECTION,
        "LEFT",
        entity_state::LEFT,
        "RIGHT",
        entity_state::RIGHT);

    fck_namespace["entityDirectionToString"] = &entity_state::directionToString;
    fck_namespace["entityDirectionFromString"] = &entity_state::directionFromString;

    fck_namespace.new_enum(
        "ComponentType",
        "NO_TYPE",
        component_type::NO_TYPE,
        "TRANSFORM",
        component_type::TRANSFORM,
        "VELOCITY",
        component_type::VELOCITY,
        "SCENE",
        component_type::SCENE,
        "PLAYER",
        component_type::PLAYER,
        "DRAWABLE",
        component_type::DRAWABLE,
        "DRAWABLE_ANIMATION",
        component_type::DRAWABLE_ANIMATION,
        "COLLISION",
        component_type::COLLISION,
        "STATE",
        component_type::STATE,
        "SOUND",
        component_type::SOUND,
        "SCRIPT",
        component_type::SCRIPT,
        "TARGET",
        component_type::TARGET,
        "TARGET_FOLLOW",
        component_type::TARGET_FOLLOW,
        "LOOK_AROUND",
        component_type::LOOK_AROUND,
        "STATS",
        component_type::STATS,
        "SKILLS",
        component_type::SKILLS,
        "DAMAGE",
        component_type::DAMAGE,
        "MARKER",
        component_type::MARKER);

    fck_namespace["componentTypeToString"] = &component_type::toString;
    fck_namespace["componentTypeFromString"] = &component_type::fromString;

    fck_namespace.new_enum(
        "TileMaterialType",
        "NO_TYPE",
        tile_material_type::NO_TYPE,
        "TILE",
        tile_material_type::TILE,
        "WOOD",
        tile_material_type::WOOD,
        "METAL",
        tile_material_type::METAL,
        "LIQUID",
        tile_material_type::LIQUID,
        "GRASS",
        tile_material_type::GRASS);

    fck_namespace["tileMaterialTypeToString"] = &tile_material_type::toString;
    fck_namespace["tileMaterialTypeFromString"] = &tile_material_type::fromString;

    fck_namespace.new_enum(
        "ChunkSide",
        "NO_SIDE",
        chunk_side::NO_SIDE,
        "LEFT",
        chunk_side::LEFT,
        "TOP",
        chunk_side::TOP,
        "RIGHT",
        chunk_side::RIGHT,
        "BOTTOM",
        chunk_side::BOTTOM);

    fck_namespace["chunkSideToString"] = &chunk_side::toString;
    fck_namespace["chunkSideFromString"] = &chunk_side::fromString;

    fck_namespace.new_enum(
        "ChunkType",
        "NO_TYPE",
        chunk_type::NO_TYPE,
        "UNKNOW",
        chunk_type::UNKNOW,
        "DEFAULT",
        chunk_type::DEFAULT,
        "BOSS",
        chunk_type::BOSS,
        "TRADER",
        chunk_type::TRADER);

    fck_namespace["chunkTypeToString"] = &chunk_type::toString;
    fck_namespace["chunkTypeFromString"] = &chunk_type::fromString;
}

} // namespace fck
