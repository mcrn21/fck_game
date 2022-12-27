#ifndef DAMAGEBASE_DUUBUFOBCSYD_H
#define DAMAGEBASE_DUUBUFOBCSYD_H

#include "../fck/entity.h"

namespace fck
{

namespace system
{
class Damage;
}

class DamageBase
{
    friend class system::Damage;

public:
    DamageBase(const Entity &entity, double interval, const Entity &source = Entity{});
    virtual ~DamageBase() = default;

    const Entity &getEntity() const;
    const Entity &getSource() const;

    double getInterval() const;
    double getElapsed() const;

    bool isReady() const;
    void finish();

protected:
    virtual void update(double delta_time) = 0;

private:
    void _update(double delta_time);

private:
    Entity m_entity;
    Entity m_source;
    double m_interval;
    double m_elapsed;
};

} // namespace fck

#endif // DAMAGEBASE_DUUBUFOBCSYD_H
