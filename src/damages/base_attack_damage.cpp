#include "base_attack_damage.h"

#include <spdlog/spdlog.h>

namespace fck
{

BaseAttackDamage::BaseAttackDamage(
    float damage, const Entity &entity, double interval, const Entity &source)
    : DamageBase{entity, interval, source}, m_damage{damage}
{
}

void BaseAttackDamage::update(double delta_time)
{
    //    spdlog::debug("damage");
}

} // namespace fck
