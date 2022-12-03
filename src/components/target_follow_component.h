#ifndef TARGETFOLLOWCOMPONENT_H
#define TARGETFOLLOWCOMPONENT_H

#include "../knowledge_base.h"

#include "SFML/System/Vector2.hpp"

#include <vector>

namespace fck
{

struct TargetFollowComponent
{
    enum State
    {
        NO_FOLLOWING,
        FOLLOW,
        LOST,
        RICHED
    };

    bool follow = false;
    float min_distance = 0.0f;
    float max_distance = 0.0f;

    State state = NO_FOLLOWING;
    std::vector<sf::Vector2i> path;
};

template<>
struct KnowledgeBase::ComponentItem<TargetFollowComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::TARGET_FOLLOW;
    }

    void init(toml::table *table)
    {
        if (table->contains("min_distance"))
            min_distance = table->at("min_distance").as_floating_point()->get();

        if (table->contains("max_distance"))
            max_distance = table->at("max_distance").as_floating_point()->get();
    }

    void create(Entity &entity)
    {
        TargetFollowComponent &component = entity.addComponent<TargetFollowComponent>();

        component.min_distance = min_distance;
        component.max_distance = max_distance;
    }

    float min_distance = 0.0f;
    float max_distance = 0.0f;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(TargetFollowComponent);

} // namespace fck

#endif // TARGETFOLLOWCOMPONENT_H
