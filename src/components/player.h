#ifndef PLAYER_PKPRRYSWBWRY_H
#define PLAYER_PKPRRYSWBWRY_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include <vector>

namespace fck
{

namespace component
{

struct Player
{
    bool need_change_target = false;
    bool view_hard_set_position = false;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Player> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::PLAYER;
    }

    void create(Entity &entity)
    {
        component::Player &component = entity.add<component::Player>();
    }
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Player);

}

} // namespace fck

#endif // PLAYER_PKPRRYSWBWRY_H
