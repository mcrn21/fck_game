#include "base_attack_skill.h"

#include "../entity_utils.h"

#include "../damages/damages.h"

#include "../components/components.h"

#include "spdlog/spdlog.h"

namespace fck
{

BaseAttackSkill::BaseAttackSkill(const std::string &skill_name, double cooldown_interval)
    : SkillBase{skill_name, cooldown_interval}
{
}

void BaseAttackSkill::apply(const Entity &entity, const Entity &target)
{
    m_entity = entity;
    m_target = target;

    if (m_target.isValid())
        m_attack_function = std::bind(&BaseAttackSkill::targetAttack, this, std::placeholders::_1);
    else
        m_attack_function = std::bind(&BaseAttackSkill::idleAttack, this, std::placeholders::_1);

    //    if (m_target.isValid())
    //    {
    //        DamageComponent &target_damage_component = m_target.component<DamageComponent>();
    //        target_damage_component.damage.reset(new BaseAttackDamage{m_target, 0.4, m_entity});
    //    }

    //    EntityUtils::setEntityState(entity, entity_state::SKILL_APPLY);
}

void BaseAttackSkill::update(double delta_time)
{
    m_attack_function(delta_time);

    if (remainedTime() == 0)
    {
        EntityUtils::setEntityState(m_entity, entity_state::IDLE);
    }
}

void BaseAttackSkill::idleAttack(double delta_time)
{
    spdlog::debug("idle attack");
}

void BaseAttackSkill::targetAttack(double delta_time)
{
    spdlog::debug("target attack");
}

} // namespace fck
