#ifndef STATS_TIUNOEBHMNBI_H
#define STATS_TIUNOEBHMNBI_H

#include "../knowledge_base.h"

namespace fck
{

namespace component
{

struct Stats
{
    float health = 100.0f;
    float max_health = 100.0f;

    float armor = 100.0f;
    float max_armor = 100.0f;

    float damage = 0.0f;

    double death_time = 0.4;
    double disappearance_time = 2;
    double death_elipsed = 0;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Stats> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::STATS;
    }

    void init(toml::table *table)
    {
        if (table->contains("health"))
            health = table->at("health").as_floating_point()->get();

        if (table->contains("max_health"))
            max_health = table->at("max_health").as_floating_point()->get();

        if (table->contains("armor"))
            armor = table->at("armor").as_floating_point()->get();

        if (table->contains("max_armor"))
            max_armor = table->at("max_armor").as_floating_point()->get();
    }

    void create(Entity &entity)
    {
        component::Stats &component = entity.add<component::Stats>();

        component.health = health;
        component.max_health = max_health;

        component.armor = armor;
        component.max_armor = max_armor;
    }

    float health = 100.0f;
    float max_health = 100.0f;

    float armor = 100.0f;
    float max_armor = 100.0f;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Stats);

}

} // namespace fck

#endif // STATS_COMPONENT_TIUNOEBHMNBI_H
