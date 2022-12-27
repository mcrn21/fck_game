#ifndef TARGET_HSTUAZIXVBAX_H
#define TARGET_HSTUAZIXVBAX_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include <vector>

namespace fck
{

namespace component
{

struct Target
{
    Entity target;
    std::vector<Entity> lookings;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Target> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::TARGET;
    }

    void init(toml::table *table)
    {
    }

    void create(Entity &entity)
    {
        component::Target &component = entity.add<component::Target>();
    }
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Target);

}

} // namespace fck

#endif // TARGET_HSTUAZIXVBAX_H
