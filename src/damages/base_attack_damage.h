#ifndef BASEATTACKDAMAGE_H
#define BASEATTACKDAMAGE_H

#include "../damage_base.h"

#include <SFML/Graphics.hpp>

namespace fck
{

class BaseAttackDamage : public DamageBase
{
public:
    BaseAttackDamage(
        float damage,
        const sf::Vector2f rebound_velocity,
        const Entity &entity,
        double interval,
        const Entity &source = Entity{});
    ~BaseAttackDamage() = default;

protected:
    void update(double delta_time);

private:
    float m_damage;
    sf::Vector2f m_rebound_velocity;
    std::pair<double, double> m_rebounce_interval;

    bool m_first_update;
    bool m_damaged;
};

} // namespace fck

#endif // BASEATTACKDAMAGE_H
