#include "skill_base.h"

namespace fck
{

SkillBase::SkillBase(const std::string &name, double cooldown_interval)
    : m_name{name}, m_cooldown_intreval{cooldown_interval}, m_remained_time{0}
{
}

const std::string &SkillBase::name() const
{
    return m_name;
}

double SkillBase::remainedTime() const
{
    return m_remained_time;
}

double SkillBase::cooldownInterval() const
{
    return m_cooldown_intreval;
}

bool SkillBase::isReady()
{
    return m_remained_time == 0;
}

void SkillBase::_apply(const Entity &entity, const Entity &target)
{
    m_remained_time = m_cooldown_intreval;
    apply(entity, target);
}

void SkillBase::_update(double delta_time)
{
    if (m_remained_time > 0)
        m_remained_time -= delta_time;

    if (m_remained_time < 0)
        m_remained_time = 0;

    update(delta_time);
}

} // namespace fck
