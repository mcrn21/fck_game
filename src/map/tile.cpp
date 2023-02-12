#include "tile.h"

namespace fck::map
{

Tile::Tile() : m_material_type{tile_material_type::NO_TYPE}
{
}

tile_material_type::Type Tile::getMaterialType() const
{
    return m_material_type;
}

void Tile::setMaterialType(tile_material_type::Type material_type)
{
    m_material_type = material_type;
}

} // namespace fck::map
