#ifndef SKILLS_COMPONENT_H
#define SKILLS_COMPONENT_H

#include "../knowledge_base.h"
#include "../skill_base.h"

#include "../fck/utilities.h"

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
        if (table->contains("skills"))
            skills = vector::tomlArrayToStringVector(table->at("skills").as_array());
    }

    void create(Entity &entity)
    {
        SkillsComponent &component = entity.addComponent<SkillsComponent>();

        for (const std::string &skill : skills)
            component.skills.push_back(
                std::unique_ptr<SkillBase>(KnowledgeBase::skill(skill)->create()));
    }

    std::vector<std::string> skills;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(SkillsComponent);

} // namespace fck

#endif // SKILLS_COMPONENT_H
