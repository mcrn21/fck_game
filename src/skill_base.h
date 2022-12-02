#ifndef SKILLBASE_H
#define SKILLBASE_H

#include "fck/entity.h"

#include <SFML/Graphics.hpp>

namespace fck
{

class SkillsSystem;

class SkillBase
{
    friend class SkillsSystem;

public:
    SkillBase(const std::string &name, double cooldown_interval);
    virtual ~SkillBase() = default;

    const std::string &name() const;

    double remainedTime() const;
    double cooldownInterval() const;
    bool isReady();

protected:
    virtual void apply(const Entity &entity, const Entity &target) = 0;
    virtual void update(double delta_time) = 0;

private:
    void _apply(const Entity &entity, const Entity &target);
    void _update(double delta_time);

private:
    std::string m_name;

    double m_remained_time;
    double m_cooldown_intreval;
};

} // namespace fck

#endif // SKILLBASE_H
