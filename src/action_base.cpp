#include "action_base.h"

namespace fck
{

ActionBase::ActionBase(double cooldown) : m_remained{0}, m_cooldown{cooldown}
{
}

ActionResultBase *ActionBase::apply(const Entity &entity, const std::vector<Entity> &targets)
{
    if (!isReady())
        return nullptr;
    m_remained = m_cooldown;
    return createResult(entity, targets);
}

double ActionBase::remained() const
{
    return m_remained;
}

double ActionBase::cooldown() const
{
    return m_cooldown;
}

bool ActionBase::isReady()
{
    return m_remained == 0;
}

void ActionBase::update(double delta_time)
{
    if (m_remained > 0)
        m_remained -= delta_time;

    if (m_remained < 0)
        m_remained = 0;
}

} // namespace fck
