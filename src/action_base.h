#ifndef ACTIONBASE_H
#define ACTIONBASE_H

#include "fck/entity.h"

#include <vector>

namespace fck
{

class ActionResultBase
{
public:
    ActionResultBase() = default;
    virtual ~ActionResultBase() = default;

    virtual bool update(double delta_time) = 0;
};

class ActionBase
{
public:
    ActionBase(double cooldown);
    virtual ~ActionBase() = default;

    ActionResultBase *apply(const Entity &entity, const std::vector<Entity> &targets);

    double remained() const;
    double cooldown() const;

    bool isReady();
    void update(double delta_time);

protected:
    virtual ActionResultBase *createResult(const Entity &entity, const std::vector<Entity> &targets)
        = 0;

private:
    double m_remained;
    double m_cooldown;
};

} // namespace fck

#endif // ACTIONBASE_H
