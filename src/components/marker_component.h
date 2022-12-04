#ifndef MARKER_COMPONENT_H
#define MARKER_COMPONENT_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

namespace fck
{

struct MarkerComponent
{
    Entity marker;
};

template<>
struct KnowledgeBase::ComponentItem<MarkerComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::MARKER;
    }

    void create(Entity &entity)
    {
        MarkerComponent &component = entity.addComponent<MarkerComponent>();
    }
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(MarkerComponent);

} // namespace fck

#endif // MARKER_COMPONENT_H
