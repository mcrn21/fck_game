#ifndef LEVELGUI_AICEWWWICILT_H
#define LEVELGUI_AICEWWWICILT_H

#include "gui_style.h"

#include "../gui_base.h"
#include "../knowledge_base.h"
#include "../level.h"

#include "../fck/entity.h"

namespace fck::gui
{

class LevelGui : public GuiBase
{
public:
    LevelGui(const Entity &player_entity);
    ~LevelGui() = default;

    void resize(const sf::Vector2f &size);

    void updatePlayerStats();
    void updatePlayerSkills();
    void updateTargetStats();

    void setRoomsMap(const Vector2D<Level::Room *> &rooms_map);
    void setRoomOpended(const sf::Vector2i &room_coord);
    void setCurrentRoom(const sf::Vector2i &room_coord);

private:
    void drawPlayerStats(sf::RenderTarget &target, const sf::RenderStates &states);
    void drawPlayerSkills(sf::RenderTarget &target, const sf::RenderStates &states);
    void drawTargetStats(sf::RenderTarget &target, const sf::RenderStates &states);

private:
    Entity m_player_entity;
    Entity m_target_entity;

    sf::Vector2f m_border_offset;
    sf::Vector2f m_scale;
    sf::Vector2f m_stats_scale;

    // player stats
    ProgressBar *m_player_hp_progress_bar;
    ProgressBar *m_player_armor_progress_bar;
    Minimap *m_minimap;

    // target stats
    ProgressBar *m_target_hp_progress_bar;

    struct Skill
    {
        KnowledgeBase::SkillItemBase *skill_item = nullptr;
        SkillBase *skill = nullptr;
        sf::Sprite sprite;
        sf::Text key_text;
    };
    std::vector<Skill> m_skills;
};

} // namespace fck::gui

#endif // LEVELGUI_AICEWWWICILT_H
