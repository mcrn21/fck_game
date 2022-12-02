#ifndef DAMAGEBASE_H
#define DAMAGEBASE_H

#include "fck/entity.h"

namespace fck
{

class DamageSystem;

class DamageBase
{
    friend class DamageSystem;

public:
    DamageBase(const Entity &entity, double interval, const Entity &source = Entity{});
    virtual ~DamageBase() = default;

    const Entity &entity() const;
    const Entity &source() const;

    double interval() const;
    double remainedTime() const;
    bool isReady() const;

protected:
    virtual void update(double delta_time) = 0;

private:
    void _update(double delta_time);

private:
    Entity m_entity;
    Entity m_source;
    double m_interval;
    double m_remained_time;
};

} // namespace fck

#endif // DAMAGEBASE_H
