#include "skill.h"

namespace fck::skill
{

Skill::Skill(
    const SkillFactory::Factory *skill_factory, const sol::table &skill_table, double cooldown)
    : m_skill_factory{skill_factory},
      m_skill_table{skill_table},
      m_cooldown{cooldown},
      m_elapsed{cooldown}
{
    m_update_function = m_skill_table["update"];

    m_skill_table["elapsed"] = m_elapsed;
    m_skill_table["cooldown"] = m_cooldown;
}

const SkillFactory::Factory *Skill::getSkillFactory() const
{
    return m_skill_factory;
}

sol::table &Skill::getScriptTable()
{
    return m_skill_table;
}

void Skill::setEntityToTable(const Entity &entity)
{
    m_skill_table["entity"] = entity;
}

double Skill::getElapsed() const
{
    return m_elapsed;
}

double Skill::getCooldown() const
{
    return m_cooldown;
}

bool Skill::isReady() const
{
    return m_elapsed == m_cooldown;
}

void Skill::start()
{
    m_elapsed = 0;
}

void Skill::finish()
{
    m_elapsed = m_cooldown;
}

void Skill::update(double delta_time)
{
    if (m_elapsed < m_cooldown)
        m_elapsed += delta_time;

    if (m_elapsed > m_cooldown)
        m_elapsed = m_cooldown;

    m_skill_table["elapsed"] = m_elapsed;

    if (m_update_function)
    {
        auto result = m_update_function(m_skill_table, delta_time);
        if (!result.valid())
        {
            sol::error err = result;
            spdlog::warn("Error at Skill::update: {}", err.what());
        }
    }
}

} // namespace fck::skill
