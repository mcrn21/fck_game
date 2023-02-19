#ifndef SKILL_NSRGVYNGFLBV_H
#define SKILL_NSRGVYNGFLBV_H

#include "../fck/entity.h"
#include "../knowledge_base/skill_factory.h"

#include <sol/sol.hpp>

namespace fck
{

namespace system
{
class Skills;
}

namespace skill
{

class Skill
{
    friend class system::Skills;

public:
    Skill(
        const SkillFactory::Factory *skill_factory, const sol::table &skill_table, double cooldown);
    ~Skill() = default;

    const SkillFactory::Factory *getSkillFactory() const;
    sol::table &getScriptTable();

    void setEntityToTable(const Entity &entity);

    double getElapsed() const;
    double getCooldown() const;

    bool isReady() const;

protected:
    void start();
    void finish();

    void update(double delta_time);

private:
    const SkillFactory::Factory *m_skill_factory;
    sol::table m_skill_table;

    sol::protected_function m_update_function;

    double m_elapsed;
    double m_cooldown;
};

} // namespace skill

} // namespace fck

#endif // SKILL_H
