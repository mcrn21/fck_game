#include "../map/map.h"
#include "bindings.h"

namespace fck
{

void bindMapToLua(sol::state &sol_state)
{
    sol::table fck_namespace = sol_state["fck"].get_or_create<sol::table>();
    sol::table map_namespace = fck_namespace["map"].get_or_create<sol::table>();

    // Tile
    sol::usertype<map::Tile> tile_bind = fck_namespace.new_usertype<map::Tile>(
        "Tile",
        sol::constructors<map::Tile()>(),
        "getMaterialType",
        &map::Tile::getMaterialType,
        "setMaterialType",
        &map::Tile::setMaterialType);

    // Vector2d_Tile
    sol::usertype<Vector2D<map::Tile>> vector2d_tile_bind
        = fck_namespace.new_usertype<Vector2D<map::Tile>>(
            "Vector2d_Tile",
            sol::constructors<
                Vector2D<map::Tile>(),
                Vector2D<map::Tile>(const sf::Vector2i &, map::Tile)>(),
            "transformIndex",
            &Vector2D<map::Tile>::transformIndex,
            "getData",
            sol::resolve<map::Tile &(const sf::Vector2i &)>(&Vector2D<map::Tile>::getData),
            "at",
            &Vector2D<map::Tile>::at,
            sol::meta_function::index,
            [](Vector2D<map::Tile> &v, int32_t index) -> map::Tile & { return v[index]; },
            sol::meta_function::new_index,
            [](Vector2D<map::Tile> &v, int32_t index, map::Tile tile) { v[index] = tile; },
            "getSize2D",
            &Vector2D<map::Tile>::getSize2D,
            "getSize",
            &Vector2D<map::Tile>::getSize,
            "clear",
            &Vector2D<map::Tile>::clear,
            "resize",
            &Vector2D<map::Tile>::resize,
            "resize",
            &Vector2D<map::Tile>::resize);

    // Chunk
    sol::usertype<map::Chunk> chunk_bind = fck_namespace.new_usertype<map::Chunk>(
        "Chunk",
        sol::constructors<map::Chunk()>(),
        "getNeighbors",
        &map::Chunk::getNeighbors,
        "setNeighbors",
        &map::Chunk::setNeighbors,
        "getEntities",
        &map::Chunk::getEntities,
        "setEntities",
        &map::Chunk::setEntities,
        "getChunkEntryEntities",
        &map::Chunk::getChunkEntryEntities,
        "setChunkEntryEntities",
        &map::Chunk::setChunkEntryEntities,
        "getWalls",
        &map::Chunk::getWalls,
        "setWalls",
        &map::Chunk::setWalls,
        "getTiles",
        &map::Chunk::getTiles,
        "setTiles",
        &map::Chunk::setTiles,
        "getType",
        &map::Chunk::getType,
        "setType",
        &map::Chunk::setType,
        "isOpen",
        &map::Chunk::isOpen,
        "setOpen",
        &map::Chunk::setOpen,
        "enable",
        &map::Chunk::enable,
        "disable",
        &map::Chunk::disable);

    // Vector2D_Chunk
    sol::usertype<Vector2D<map::Chunk *>> vector2d_chunk_bind
        = fck_namespace.new_usertype<Vector2D<map::Chunk *>>(
            "Vector2d_Tile",
            sol::constructors<
                Vector2D<map::Chunk *>(),
                Vector2D<map::Chunk *>(const sf::Vector2i &, map::Chunk *)>(),
            "transformIndex",
            &Vector2D<map::Chunk *>::transformIndex,
            "getData",
            sol::resolve<map::Chunk *&(const sf::Vector2i &)>(&Vector2D<map::Chunk *>::getData),
            "at",
            &Vector2D<map::Chunk *>::at,
            sol::meta_function::index,
            [](Vector2D<map::Chunk *> &v, int32_t index) -> map::Chunk *& { return v[index]; },
            sol::meta_function::new_index,
            [](Vector2D<map::Chunk *> &v, int32_t index, map::Chunk *tile) { v[index] = tile; },
            "getSize2D",
            &Vector2D<map::Chunk *>::getSize2D,
            "getSize",
            &Vector2D<map::Chunk *>::getSize,
            "clear",
            &Vector2D<map::Chunk *>::clear,
            "resize",
            &Vector2D<map::Chunk *>::resize,
            "resize",
            &Vector2D<map::Chunk *>::resize);

    // Map
    sol::usertype<map::Map> map_bind = fck_namespace.new_usertype<map::Map>(
        "Tile",
        sol::constructors<map::Map(b2::DynamicTree<Entity> *)>(),
        "getChunks",
        &map::Map::getChunks,
        "getTileSize",
        &map::Map::getTileSize,
        "getAreaSize",
        &map::Map::getAreaSize,
        "getChunkSize",
        &map::Map::getChunkSize,
        "getFirstChunkCoords",
        &map::Map::getFirstChunkCoords,
        "getCurrentChunkCoords",
        &map::Map::getCurrentChunkCoords,
        "getCurrentChunk",
        &map::Map::getCurrentChunk,
        "setCurrentChunk",
        [](map::Map &map, const sf::Vector2i &chunk_coords, const sol::object &entities_object) {
            std::vector<Entity> entities = entities_object.as<std::vector<Entity>>();
            map.setCurrentChunk(chunk_coords, entities);
        },
        "tileCoordsByPosition",
        &map::Map::tileCoordsByPosition);
}

} // namespace fck
