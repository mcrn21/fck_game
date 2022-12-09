#ifndef MARKER_XORSSWDVTXIP_H
#define MARKER_XORSSWDVTXIP_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

namespace fck
{

namespace component
{

struct Marker
{
    Entity marker;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Marker> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::MARKER;
    }

    void create(Entity &entity)
    {
        component::Marker &component = entity.addComponent<component::Marker>();
    }
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Marker);

}

} // namespace fck

#endif // MARKER_XORSSWDVTXIP_H
