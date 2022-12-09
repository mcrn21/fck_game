#ifndef COLLISION_YEOQUPNYJSJT_H
#define COLLISION_YEOQUPNYJSJT_H

#include "../fck_common.h"
#include "../knowledge_base.h"

#include <vector>

namespace fck
{

namespace component
{

struct Collision
{
    bool wall = false;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Collision> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::COLLISION;
    }

    void init(toml::table *table)
    {
        if (table->contains("wall"))
            wall = table->at("wall").as_boolean()->get();
    }

    void create(Entity &entity)
    {
        component::Collision &component = entity.addComponent<component::Collision>();

        component.wall = wall;
    }

    bool wall = false;
};

namespace component
{
KNOWLEDGE_BASE_REGISTER_COMPONENT(Collision);
}

} // namespace fck

#endif // COLLISION_YEOQUPNYJSJT_H
