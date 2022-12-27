#ifndef SKILLBASE_HZOZKXBOINCQ_H
#define SKILLBASE_HZOZKXBOINCQ_H

#include "../fck/entity.h"

#include <SFML/Graphics.hpp>

namespace fck
{

namespace system
{
class Skills;
}

class SkillBase
{
    friend class system::Skills;

public:
    SkillBase(const std::string &name, double cooldown);
    virtual ~SkillBase() = default;

    const std::string &getName() const;

    double getEelapsed() const;
    double getCooldown() const;
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

#endif // SKILLBASE_HZOZKXBOINCQ_H
