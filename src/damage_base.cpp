#include "damage_base.h"

namespace fck
{

DamageBase::DamageBase(const Entity &entity, double interval, const Entity &source)
    : m_entity{entity}, m_source{source}, m_interval{interval}, m_remained_time{interval}
{
}

const Entity &DamageBase::entity() const
{
    return m_entity;
}

const Entity &DamageBase::source() const
{
    return m_source;
}

double DamageBase::interval() const
{
    return m_interval;
}

double DamageBase::remainedTime() const
{
    return m_remained_time;
}

bool DamageBase::isReady() const
{
    return m_remained_time == 0;
}

void DamageBase::_update(double delta_time)
{
    if (m_remained_time > 0)
        m_remained_time -= delta_time;

    if (m_remained_time < 0)
        m_remained_time = 0;

    update(delta_time);
}

} // namespace fck
