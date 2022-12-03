#ifndef BASEATTACKSKILL_H
#define BASEATTACKSKILL_H

#include "../knowledge_base.h"
#include "../skill_base.h"

#include "../fck/entity.h"
#include "../fck/utilities.h"

#include "toml++/toml.h"

#include <functional>

namespace fck
{

class BaseAttackSkill : public SkillBase
{
public:
    BaseAttackSkill(
        const std::string &skill_name,
        double cooldown,
        const std::pair<float, float> &damage,
        const std::vector<std::string> &attack_animations,
        const std::pair<double, double> &jump_interval,
        const sf::Vector2f &target_jump_offset,
        const sf::Vector2f &idle_jump_offset);
    ~BaseAttackSkill() = default;

protected:
    void apply(const Entity &entity, const Entity &target);
    void update(double delta_time);
    void finished();

private:
    void idleAttack(double delta_time);
    void targetAttack(double delta_time);

private:
    Entity m_entity;
    Entity m_target;

    std::pair<float, float> m_damage;

    std::function<void(double)> m_attack_function;

    std::vector<std::string> m_attack_animations;

    std::pair<double, double> m_jump_interval;
    sf::Vector2f m_jump_point;
    sf::Vector2f m_jump_velocity;

    sf::Vector2f m_target_jump_offset;
    sf::Vector2f m_idle_jump_offset;

    bool m_target_attacked;
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
        return m_display_description;
    }

    const std::string &textureName() const
    {
        return m_texture_name;
    }

    const sf::IntRect &textureRect() const
    {
        return m_texture_rect;
    }

    SkillBase *create() const
    {
        return new BaseAttackSkill(
            m_name,
            m_cooldown,
            m_damage,
            m_attack_animations,
            m_jump_interval,
            m_target_jump_offset,
            m_idle_jump_offset);
    }

private:
    void init(toml::table *table)
    {
        m_name = table->at("name").as_string()->get();

        m_display_name = table->at("display_name").as_string()->get();
        m_display_description = table->at("display_description").as_string()->get();
        m_texture_name = table->at("texture_name").as_string()->get();
        m_texture_rect = rect::tomlArrayToIntRect(table->at("texture_rect").as_array());

        m_cooldown = table->at("cooldown").as_floating_point()->get();
        m_damage = pair::tomlArrayToFloatPair(table->at("damage").as_array());
        m_attack_animations
            = vector::tomlArrayToStringVector(table->at("attack_animations").as_array());
        m_jump_interval = pair::tomlArrayToDoublePair(table->at("jump_interval").as_array());
        m_target_jump_offset
            = vector2::tomlArrayToVector2f(table->at("target_jump_offset").as_array());
        m_idle_jump_offset = vector2::tomlArrayToVector2f(table->at("idle_jump_offset").as_array());
    }

private:
    std::string m_name;

    std::string m_display_name;
    std::string m_display_description;
    std::string m_texture_name;
    sf::IntRect m_texture_rect;

    double m_cooldown = 0.0;
    std::pair<float, float> m_damage;
    std::vector<std::string> m_attack_animations;
    std::pair<double, double> m_jump_interval = {0.0f, 0.0f};
    sf::Vector2f m_target_jump_offset;
    sf::Vector2f m_idle_jump_offset;
};

KNOWLEDGE_BASE_REGISTER_BASE_SKILL(BaseAttackSkill, "base_attack");

} // namespace fck

#endif // BASEATTACKSKILL_H
