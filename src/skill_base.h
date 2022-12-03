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
    SkillBase(const std::string &name, double cooldown);
    virtual ~SkillBase() = default;

    const std::string &name() const;

    double elapsed() const;
    double cooldown() const;
    bool isReady();
    void finish();
    void broke();

protected:
    virtual void apply(const Entity &entity, const Entity &target) = 0;
    virtual void update(double delta_time) = 0;

private:
    void _apply(const Entity &entity, const Entity &target);
    void _update(double delta_time);

private:
    std::string m_name;

    double m_elapsed;
    double m_cooldown;
    float m_broken;
};

} // namespace fck

#endif // SKILLBASE_H
