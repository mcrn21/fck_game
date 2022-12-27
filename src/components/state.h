#ifndef STATE_LBAUWXMMAILP_H
#define STATE_LBAUWXMMAILP_H

#include "../fck_common.h"
#include "../knowledge_base.h"

#include <functional>

namespace fck
{

namespace component
{

struct State
{
    entity_state::State state = entity_state::IDLE;
    entity_state::Direction direction = entity_state::RIGHT;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::State> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::STATE;
    }

    void init(toml::table *table)
    {
        if (table->contains("state"))
            state = entity_state::stateFromString(table->at("state").as_string()->get());

        if (table->contains("direction"))
            direction
                = entity_state::directionFromString(table->at("direction").as_string()->get());
    }

    void create(Entity &entity)
    {
        component::State &component = entity.add<component::State>();

        component.state = state;
        component.direction = direction;
    }

    entity_state::State state = entity_state::IDLE;
    entity_state::Direction direction = entity_state::RIGHT;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(State);

}

} // namespace fck

#endif // STATECOMPONENT_LBAUWXMMAILP_H
