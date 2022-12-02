#ifndef STATS_COMPONENT_H
#define STATS_COMPONENT_H

#include "../knowledge_base.h"

namespace fck
{

struct StatsComponent
{
    float health = 100.0f;
    float max_health = 100.0f;

    float armor = 43.0f;
    float max_armor = 100.0f;

    float damage = 0.0f;

    double death_elipsed = 0;
    double death_time = 0.400;
    double disappearance_time = 2;
};

template<>
struct KnowledgeBase::ComponentItem<StatsComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::STATS;
    }

    void init(toml::table *table)
    {
    }

    void create(Entity &entity)
    {
        StatsComponent &component = entity.addComponent<StatsComponent>();
    }
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(StatsComponent);

} // namespace fck

#endif // STATS_COMPONENT_H
