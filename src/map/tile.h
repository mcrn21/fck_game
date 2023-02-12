#ifndef TILE_HKSDTVRRNYAF_H
#define TILE_HKSDTVRRNYAF_H

#include "../fck_common.h"

namespace fck::map
{

class Tile
{
public:
    Tile();
    ~Tile() = default;

    tile_material_type::Type getMaterialType() const;
    void setMaterialType(tile_material_type::Type material_type);

private:
    tile_material_type::Type m_material_type;
};

} // namespace fck::map

#endif // TILE_HKSDTVRRNYAF_H
