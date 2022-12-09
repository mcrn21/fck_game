#ifndef DAMAGE_PDRYABUEXDAD_H
#define DAMAGE_PDRYABUEXDAD_H

#include "../damage_base.h"
#include "../knowledge_base.h"

#include <memory>

namespace fck
{

namespace component
{

struct Damage
{
    std::unique_ptr<DamageBase> damage;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Damage> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::DAMAGE;
    }

    void create(Entity &entity)
    {
        component::Damage &component = entity.addComponent<component::Damage>();
    }
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Damage);

}

} // namespace fck

#endif // DAMAGE_PDRYABUEXDAD_H
