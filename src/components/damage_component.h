#ifndef DAMAGECOMPONENT_H
#define DAMAGECOMPONENT_H

#include "../damage_base.h"
#include "../knowledge_base.h"

#include <memory>

namespace fck
{

struct DamageComponent
{
    std::unique_ptr<DamageBase> damage;
};

template<>
struct KnowledgeBase::ComponentItem<DamageComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::DAMAGE;
    }

    void create(Entity &entity)
    {
        DamageComponent &component = entity.addComponent<DamageComponent>();
    }
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(DamageComponent);

} // namespace fck

#endif // DAMAGECOMPONENT_H
