#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "../fck_common.h"
#include "../knowledge_base.h"

namespace fck
{

struct CollisionComponent
{
    collision_type::Type type = collision_type::DYNAMIC;
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
        if (table->contains("type"))
            type = collision_type::fromString(table->at("type").as_string()->get());
    }

    void create(Entity &entity)
    {
        CollisionComponent &component = entity.addComponent<CollisionComponent>();

        component.type = type;
    }

    collision_type::Type type = collision_type::DYNAMIC;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(CollisionComponent);

} // namespace fck

#endif // COLLISIONCOMPONENT_H
