#ifndef TYPE_COMPONENT_H
#define TYPE_COMPONENT_H

#include "../knowledge_base.h"

#include "../fck_common.h"

namespace fck
{

struct TypeComponent
{
    entity_type::Type type = entity_type::NO_TYPE;
};

template<>
struct KnowledgeBase::ComponentItem<TypeComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::TYPE;
    }

    void init(toml::table *table)
    {
        if (table->contains("type"))
            type = entity_type::fromString(table->at("type").as_string()->get());
    }

    void create(Entity &entity)
    {
        TypeComponent &component = entity.addComponent<TypeComponent>();

        component.type = type;
    }

    entity_type::Type type;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(TypeComponent);

} // namespace fck

#endif // TYPE_COMPONENT_H
