#ifndef TARGET_COMPONENT_H
#define TARGET_COMPONENT_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

namespace fck
{

struct TargetComponent
{
    ~TargetComponent()
    {
        if (target_mark.isValid())
            target_mark.destroy();
    }

    Entity target;
    Entity target_mark;
};

template<>
struct KnowledgeBase::ComponentItem<TargetComponent> : ComponentItemBase
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
        TargetComponent &component = entity.addComponent<TargetComponent>();
    }
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(TargetComponent);

} // namespace fck

#endif // TARGET_COMPONENT_H
