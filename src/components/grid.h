#ifndef GRID_HONAMXSCVLOL_H
#define GRID_HONAMXSCVLOL_H

#include "../fck_common.h"
#include "../knowledge_base.h"

namespace fck
{

namespace component
{

struct Grid
{
    tile_material_type::Type tile_material;
    sf::Vector2i position;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Grid> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::GRID;
    }

    void init(toml::table *table)
    {
    }

    void create(Entity &entity)
    {
        component::Grid &component = entity.add<component::Grid>();
    }
};

namespace component
{
KNOWLEDGE_BASE_REGISTER_COMPONENT(Grid);
}

} // namespace fck

#endif // GRID_HONAMXSCVLOL_H
