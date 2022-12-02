#ifndef BASEATTACKDAMAGE_H
#define BASEATTACKDAMAGE_H

#include "../damage_base.h"

namespace fck
{

class BaseAttackDamage : public DamageBase
{
public:
    BaseAttackDamage(
        float damage, const Entity &entity, double interval, const Entity &source = Entity{});
    ~BaseAttackDamage() = default;

protected:
    void update(double delta_time);

private:
    float m_damage;
};

} // namespace fck

#endif // BASEATTACKDAMAGE_H
