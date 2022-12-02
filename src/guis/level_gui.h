#ifndef LEVELGUI_H
#define LEVELGUI_H

#include "../gui_base.h"

#include "../knowledge_base.h"

#include "../fck/entity.h"

#include <SFML/Graphics.hpp>

namespace fck
{

class LevelGui : public GuiBase
{
public:
    LevelGui(sf::View *view, const Entity &player_entity);
    ~LevelGui() = default;

    void draw();

private:
    void drawPlayerStats(const Entity &entity);
    void drawPlayerSkills(const Entity &entity);
    void drawTargetStats(const Entity &entity);

private:
    sf::View *m_view;
    Entity m_player_entity;

    sf::Vector2f m_scale;
    sf::Vector2f m_border_offset;

    sf::Vector2f m_player_stats_window_size;
    sf::Vector2f m_target_stats_window_size;

    // player stats
    sf::Sprite m_player_stats_bg_sprite;

    // target stats
    sf::Sprite m_target_stats_bg_sprite;
    sf::Sprite m_target_skull_sprite;

    // common
    sf::Sprite m_stats_hp_bar_sprite;
    float m_stats_hp_bar_width;

    sf::Sprite m_stats_armor_bar_sprite;
    float m_stats_armor_bar_width;

    struct Skill
    {
        KnowledgeBase::SkillItemBase *skill_item = nullptr;
        SkillBase *skill = nullptr;
        sf::Sprite sprite;
    };
    std::vector<Skill> m_skills;
};

} // namespace fck

#endif // LEVELGUI_H
