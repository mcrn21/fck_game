#ifndef LEVELWIDGET_SKMCNUWIVHCR_H
#define LEVELWIDGET_SKMCNUWIVHCR_H

#include "../../fck/entity.h"
#include "../../sigslot/signal.hpp"
#include "../minimap.h"
#include "../progress_bar.h"
#include "../skill_icon.h"
#include "../widget.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace fck::gui
{

class LevelWidget : public Widget, public sigslot::observer_st
{
public:
    LevelWidget(Widget *parent = nullptr);
    ~LevelWidget();

    const Entity &getPlayerEntity() const;
    void setPlayerEntity(const Entity &entity);

    const Entity &getTargetEntity() const;
    void setTargetEntity(const Entity &entity);

    void updatePlayerStats();
    void updatePlayerSkills();
    void updateTargetStats();

    void setChunks(const Vector2D<map::Chunk *> &chunks);

public: //slots
    void onEntityTargetChanged(
        const Entity &entity, const Entity &target, const Entity &old_target);
    void onEntityHealthChanged(const Entity &entity, float);
    void onEntityArmorChanged(const Entity &entity, float);
    void onEntitySkillApplied(const Entity &entity, SkillBase *);
    void onEntitySkillFinished(const Entity &entity, SkillBase *);
    void onChunkOpened(const sf::Vector2i &chunk_coords);
    void onChunkChanged(const sf::Vector2i &chunk_coords);

protected:
    void onWindowResized(const sf::Vector2f &size) override;

private:
    void updateGeometry();

private:
    Entity m_player_entity;
    Entity m_target_entity;

    ProgressBar *m_player_hp_progress_bar;
    ProgressBar *m_player_armor_progress_bar;
    Minimap *m_minimap;

    std::vector<std::unique_ptr<SkillIcon>> m_player_skill_icons;

    ProgressBar *m_target_hp_progress_bar;
};

} // namespace fck::gui

#endif // LEVELWIDGET_SKMCNUWIVHCR_H
