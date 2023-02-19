#ifndef SKILLFACTORY_H
#define SKILLFACTORY_H

#include <sol/sol.hpp>

#include <SFML/Graphics.hpp>

namespace fck
{

namespace skill
{
class Skill;
}

class SkillFactory
{
public:
    struct Factory
    {
        friend class SkillFactory;

    public:
        std::string getSkillDisplayName() const;
        std::string getSkillDisplayDescription() const;
        std::string getSkillTextureName() const;
        sf::IntRect getSkillTextureRect() const;

        skill::Skill *createSkill();

    private:
        sol::table m_skill_table;
    };

    static void setSolState(sol::state *sol_state);
    static void registerSkillFactory(const std::string &skill_name, const std::string &skill_str);
    static skill::Skill *createSkill(const std::string &skill_name);
    static const Factory *getSkillFactory(const std::string &skill_name);

private:
    static SkillFactory &instance();

    SkillFactory();
    ~SkillFactory() = default;

private:
    sol::state *m_sol_state;
    std::unordered_map<std::string, std::unique_ptr<Factory>> m_skill_factories;
};

} // namespace fck

#endif // SKILLFACTORY_H
