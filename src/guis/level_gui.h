#ifndef LEVELGUI_H
#define LEVELGUI_H

#include "../gui_base.h"
#include "../knowledge_base.h"

#include "../fck/entity.h"

namespace fck
{

class LevelGui : public GuiBase
{
public:
    LevelGui(const sf::Vector2f &size, const Entity &player_entity);
    ~LevelGui() = default;

    void resize(const sf::Vector2f &size);
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

    void updatePlayerStats();
    void updatePlayerSkills();
    void updateTargetStats();

private:
    void drawPlayerStats(sf::RenderTarget &target, const sf::RenderStates &states);
    void drawPlayerSkills(sf::RenderTarget &target, const sf::RenderStates &states);
    void drawTargetStats(sf::RenderTarget &target, const sf::RenderStates &states);

private:
    sf::Vector2f m_size;
    Entity m_player_entity;

    sf::Vector2f m_border_offset;
    sf::Vector2f m_scale;
    sf::Vector2f m_stats_scale;

    float m_stats_hp_bar_width;
    float m_stats_armor_bar_width;

    // player stats
    sf::Sprite m_player_stats_bg_sprite;
    sf::Sprite m_player_hp_bar_sprite;
    sf::Sprite m_player_armor_bar_sprite;
    sf::Text m_player_hp_text;
    sf::Text m_player_armor_text;

    // target stats
    sf::Sprite m_target_stats_bg_sprite;
    sf::Sprite m_target_hp_bar_sprite;
    sf::Text m_target_hp_text;

    struct Skill
    {
        KnowledgeBase::SkillItemBase *skill_item = nullptr;
        SkillBase *skill = nullptr;
        sf::Sprite sprite;
        sf::Text key_text;
    };
    std::vector<Skill> m_skills;
};

} // namespace fck

#endif // LEVELGUI_H
