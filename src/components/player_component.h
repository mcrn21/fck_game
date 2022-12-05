#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include <vector>

namespace fck
{

struct PlayerComponent
{
    bool need_change_target = false;
    bool view_hard_set_position = false;
};

template<>
struct KnowledgeBase::ComponentItem<PlayerComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::PLAYER;
    }

    void create(Entity &entity)
    {
        PlayerComponent &component = entity.addComponent<PlayerComponent>();
    }
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(PlayerComponent);

} // namespace fck

#endif // PLAYERCOMPONENT_H
