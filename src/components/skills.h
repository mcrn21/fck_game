#ifndef SKILLS_GJSWDNDSLMFP_H
#define SKILLS_GJSWDNDSLMFP_H

#include "../knowledge_base.h"
#include "../skills/skill_base.h"

#include "../fck/utilities.h"

#include <memory>
#include <vector>

namespace fck
{

namespace component
{

struct Skills
{
    int32_t next_skill = -1;
    std::vector<std::unique_ptr<SkillBase>> skills;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Skills> : ComponentItemBase
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
        component::Skills &component = entity.add<component::Skills>();

        for (const std::string &skill : skills)
            component.skills.push_back(
                std::unique_ptr<SkillBase>(KnowledgeBase::getSkill(skill)->create()));
    }

    std::vector<std::string> skills;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Skills);

}

} // namespace fck

#endif // SKILLS_COMPONENT_H
