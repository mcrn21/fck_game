#ifndef BASEATTACKSKILL_H
#define BASEATTACKSKILL_H

#include "../knowledge_base.h"
#include "../skill_base.h"

#include "../fck/entity.h"

#include <functional>

namespace fck
{

class BaseAttackSkill : public SkillBase
{
public:
    BaseAttackSkill(const std::string &skill_name, double cooldown_interval);
    ~BaseAttackSkill() = default;

protected:
    void apply(const Entity &entity, const Entity &target);
    void update(double delta_time);

private:
    void idleAttack(double delta_time);
    void targetAttack(double delta_time);

private:
    Entity m_entity;
    Entity m_target;

    std::function<void(double)> m_attack_function;
};

template<>
struct KnowledgeBase::SkillItem<BaseAttackSkill> : SkillItemBase
{
public:
    const std::string &name() const
    {
        return m_name;
    }

    const std::string &displayName() const
    {
        return m_display_name;
    }

    const std::string &displayDescription() const
    {
        return m_display_discription;
    }

    const std::string &textureName() const
    {
        return m_texture_name;
    }

    const sf::IntRect &textureRect() const
    {
        return m_texture_reect;
    }

    SkillBase *create() const
    {
        return new BaseAttackSkill(m_name, 0.4);
    }

private:
    std::string m_name = "base_attack";
    std::string m_display_name = "Attack";
    std::string m_display_discription = "Base attack skill";
    std::string m_texture_name = "hud";
    sf::IntRect m_texture_reect = {{4, 184}, {82, 88}};
};

KNOWLEDGE_BASE_REGISTER_SKILL(BaseAttackSkill);

} // namespace fck

#endif // BASEATTACKSKILL_H
