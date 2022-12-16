#ifndef TARGETFOLLOW_XFKEXDVWZDFN_H
#define TARGETFOLLOW_XFKEXDVWZDFN_H

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include "SFML/System/Vector2.hpp"

#include <variant>
#include <vector>

namespace fck
{

namespace component
{

struct TargetFollow
{
    enum State
    {
        LOST,
        RICHED
    };

    bool follow = false;
    float min_distance = 0.0f;
    State state = LOST;
    std::vector<sf::Vector2i> path;
    sf::Vector2f target;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::TargetFollow> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::TARGET_FOLLOW;
    }

    void init(toml::table *table)
    {
        if (table->contains("min_distance"))
            min_distance = table->at("min_distance").as_floating_point()->get();
    }

    void create(Entity &entity)
    {
        component::TargetFollow &component = entity.addComponent<component::TargetFollow>();

        component.min_distance = min_distance;
    }

    float min_distance = 0.0f;
    float max_distance = 0.0f;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(TargetFollow);

}

} // namespace fck

#endif // TARGETFOLLOW_XFKEXDVWZDFN_H
