#include "skill_base.h"

namespace fck
{

SkillBase::SkillBase(const std::string &name, double cooldown)
    : m_name{name}, m_cooldown{cooldown}, m_elapsed{cooldown}, m_broken{false}
{
}

const std::string &SkillBase::getName() const
{
    return m_name;
}

double SkillBase::getEelapsed() const
{
    return m_elapsed;
}

double SkillBase::getCooldown() const
{
    return m_cooldown;
}

bool SkillBase::isReady()
{
    return m_elapsed == m_cooldown;
}

void SkillBase::finish()
{
    m_elapsed = m_cooldown;
}

void SkillBase::broke()
{
    m_broken = true;
}

void SkillBase::_apply(const Entity &entity, const Entity &target)
{
    m_elapsed = 0;
    m_broken = false;
    apply(entity, target);
}

void SkillBase::_update(double delta_time)
{
    if (m_elapsed < m_cooldown)
        m_elapsed += delta_time;

    if (m_elapsed > m_cooldown)
        m_elapsed = m_cooldown;

    if (!m_broken)
        update(delta_time);
}

} // namespace fck
