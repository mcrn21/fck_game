#ifndef STATECOMPONENT_H
#define STATECOMPONENT_H

#include "../fck_common.h"
#include "../knowledge_base.h"

#include <functional>

namespace fck
{

struct StateComponent
{
    entity_state::State state = entity_state::IDLE;
    entity_state::Direction direction = entity_state::RIGHT;
};

template<>
struct KnowledgeBase::ComponentItem<StateComponent> : ComponentItemBase
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
        StateComponent &component = entity.addComponent<StateComponent>();

        component.state = state;
        component.direction = direction;
    }

    entity_state::State state = entity_state::IDLE;
    entity_state::Direction direction = entity_state::RIGHT;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(StateComponent);

} // namespace fck

#endif // STATECOMPONENT_H
