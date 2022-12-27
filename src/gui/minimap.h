#ifndef MINIMAP_ESYFJTFZLGFX_H
#define MINIMAP_ESYFJTFZLGFX_H

#include "box.h"
#include "grid.h"
#include "widget.h"

#include "../level.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Minimap : public Widget
{
public:
    Minimap(Widget *parent = nullptr);
    ~Minimap() = default;

    const Vector2D<Room *> *getRoomsMap() const;
    void setRoomsMap(const Vector2D<Room *> &rooms_map);

    void setRoomOpened(const sf::Vector2i &room_coord);
    void setCurrentRoom(const sf::Vector2i &room_coord);

protected:
    void onResized(const sf::Vector2f &size) override;
    void onThemeChanged(const WidgetTheme &widget_theme) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateGeometry();

private:
    const Vector2D<Room *> *m_rooms_map;

    Box m_background;

    Grid m_rooms_map_grid;
    std::unordered_map<int32_t, int32_t> m_room_map_grid_texture_indexes;

    Grid m_rooms_type_grid;
    std::unordered_map<int32_t, int32_t> m_room_type_grid_texture_indexes;

    Box m_current_room_highlight;

    sf::Vector2i m_current_room;
};

} // namespace fck::gui

#endif // MINIMAP_ESYFJTFZLGFX_H
