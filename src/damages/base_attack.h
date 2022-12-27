#ifndef BASEATTACK_MOWQSSIUNMOR_H
#define BASEATTACK_MOWQSSIUNMOR_H

#include "../damages/damage_base.h"

#include <SFML/Graphics.hpp>

namespace fck::damage
{

class BaseAttack : public DamageBase
{
public:
    BaseAttack(
        float damage,
        const sf::Vector2f rebound_velocity,
        const Entity &entity,
        double interval,
        const Entity &source = Entity{});
    ~BaseAttack() = default;

protected:
    void update(double delta_time);

private:
    float m_damage;
    sf::Vector2f m_rebound_velocity;
    std::pair<double, double> m_rebounce_interval;

    bool m_first_update;
    bool m_damaged;
};

} // namespace fck::damage

#endif // BASEATTACK_MOWQSSIUNMOR_H
