#ifndef LEVELWIDGET_SKMCNUWIVHCR_H
#define LEVELWIDGET_SKMCNUWIVHCR_H

#include "../minimap.h"
#include "../progress_bar.h"
#include "../widget.h"

#include "../../fck/entity.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class LevelWidget : public Widget
{
public:
    LevelWidget(Widget *parent = nullptr);
    ~LevelWidget() = default;

    const Entity &getPlayerEntity() const;
    void setPlayerEntity(const Entity &entity);

    const Entity &getTargetEntity() const;
    void setTargetEntity(const Entity &entity);

    void updatePlayerStats();
    void updateTargetStats();

    void setRoomsMap(const Vector2D<Room *> &rooms_map);
    void setRoomOpended(const sf::Vector2i &room_coord);
    void setCurrentRoom(const sf::Vector2i &room_coord);

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

    ProgressBar *m_target_hp_progress_bar;
};

} // namespace fck::gui

#endif // LEVELWIDGET_SKMCNUWIVHCR_H
