#ifndef SKILLS_COMPONENT_H
#define SKILLS_COMPONENT_H

#include "../knowledge_base.h"
#include "../skill_base.h"

#include <memory>
#include <vector>

namespace fck
{

struct SkillsComponent
{
    int32_t next_skill = -1;
    std::vector<std::unique_ptr<SkillBase>> skills;
};

template<>
struct KnowledgeBase::ComponentItem<SkillsComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SKILLS;
    }

    void init(toml::table *table)
    {
    }

    void create(Entity &entity)
    {
        SkillsComponent &component = entity.addComponent<SkillsComponent>();
    }
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(SkillsComponent);

} // namespace fck

#endif // SKILLS_COMPONENT_H
