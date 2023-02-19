#include "skill_factory.h"
#include "../skills/skill.h"

#include <spdlog/spdlog.h>

namespace fck
{

std::string SkillFactory::Factory::getSkillDisplayName() const
{
    return m_skill_table["display_name"];
}

std::string SkillFactory::Factory::getSkillDisplayDescription() const
{
    return m_skill_table["display_description"];
}

std::string SkillFactory::Factory::getSkillTextureName() const
{
    return m_skill_table["texture_name"];
}

sf::IntRect SkillFactory::Factory::getSkillTextureRect() const
{
    return m_skill_table["texture_rect"];
}

skill::Skill *SkillFactory::Factory::createSkill()
{
    auto result = m_skill_table["create"]();
    if (!result.valid())
    {
        sol::error err = result;
        spdlog::warn("Create skill error: {}", err.what());
        return nullptr;
    }

    return new skill::Skill{this, result, m_skill_table["cooldown"]};
}

void SkillFactory::setSolState(sol::state *sol_state)
{
    instance().m_sol_state = sol_state;
}

void SkillFactory::registerSkillFactory(const std::string &skill_name, const std::string &skill_str)
{
    if (!instance().m_sol_state)
        return;

    sol::load_result load_result = instance().m_sol_state->load(skill_str);

    if (!load_result.valid())
    {
        sol::error err = load_result;
        spdlog::warn("Register skill error: {}", err.what());
        return;
    }

    auto script_result = load_result();
    if (!script_result.valid())
    {
        sol::error err = script_result;
        spdlog::warn("Register skill error: {}", err.what());
        return;
    }

    std::unique_ptr<Factory> script_factory = std::make_unique<Factory>();

    script_factory->m_skill_table = script_result;
    instance().m_skill_factories[skill_name] = std::move(script_factory);

    spdlog::info("Register skill factory: {}", skill_name);
}

skill::Skill *SkillFactory::createSkill(const std::string &skill_name)
{
    auto skill_factories_found = instance().m_skill_factories.find(skill_name);
    if (skill_factories_found == instance().m_skill_factories.end())
        return nullptr;
    return skill_factories_found->second->createSkill();
}

const SkillFactory::Factory *SkillFactory::getSkillFactory(const std::string &skill_name)
{
    auto skill_factories_found = instance().m_skill_factories.find(skill_name);
    if (skill_factories_found == instance().m_skill_factories.end())
        return nullptr;
    return skill_factories_found->second.get();
}

SkillFactory &SkillFactory::instance()
{
    static SkillFactory skill_factory;
    return skill_factory;
}

SkillFactory::SkillFactory() : m_sol_state{nullptr}
{
}

} // namespace fck
