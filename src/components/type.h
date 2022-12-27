#ifndef TYPE_YMWBZYXCAKCT_H
#define TYPE_YMWBZYXCAKCT_H

#include "../knowledge_base.h"

#include "../fck_common.h"

namespace fck
{

namespace component
{

struct Type
{
    entity_type::Type type = entity_type::NO_TYPE;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Type> : ComponentItemBase
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
        component::Type &component = entity.add<component::Type>();

        component.type = type;
    }

    entity_type::Type type;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Type);

}

} // namespace fck

#endif // TYPE_YMWBZYXCAKCT_H
