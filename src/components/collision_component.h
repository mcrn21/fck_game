#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "../fck_common.h"
#include "../knowledge_base.h"

#include <vector>

namespace fck
{

struct CollisionComponent
{
    bool wall = false;
};

template<>
struct KnowledgeBase::ComponentItem<CollisionComponent> : ComponentItemBase
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
        CollisionComponent &component = entity.addComponent<CollisionComponent>();

        component.wall = wall;
    }

    bool wall = false;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(CollisionComponent);

} // namespace fck

#endif // COLLISIONCOMPONENT_H
