#include "minimap.h"

#include "../fck/clipping.h"
#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck::gui
{

Minimap::Minimap(Widget *parent) : Widget{parent}
{
    setWidgetTheme(WidgetTheme::get<Minimap>());
}

const Vector2D<Room *> *Minimap::getRoomsMap() const
{
    return m_rooms_map;
}

void Minimap::setRoomsMap(const Vector2D<Room *> &rooms_map)
{
    m_rooms_map = &rooms_map;
    m_rooms_map_grid.setSize(m_rooms_map->getSize2D());
    m_rooms_type_grid.setSize(m_rooms_map->getSize2D());

    for (int32_t i = 0; i < m_rooms_map->getSize(); ++i)
    {
        auto coord = m_rooms_map->transformIndex(i);
        m_rooms_map_grid.setCellTexture(
            coord,
            m_rooms_map->at(i) ? m_room_map_grid_texture_indexes[m_rooms_map->at(i)->getNeighbors()]
                               : -1);

        m_rooms_type_grid.setCellTexture(
            coord,
            m_rooms_map->at(i) ? m_room_type_grid_texture_indexes
                    [m_rooms_map->at(i)->isOpen() ? m_rooms_map->at(i)->getType()
                                                  : room_type::UNKNOW]
                               : -1);
    }
}

void Minimap::setRoomOpened(const sf::Vector2i &room_coord)
{
    if (!m_rooms_map)
        return;

    m_rooms_type_grid.setCellTexture(
        room_coord, m_room_type_grid_texture_indexes[m_rooms_map->getData(room_coord)->getType()]);
}

void Minimap::setCurrentRoom(const sf::Vector2i &room_coord)
{
    m_current_room = room_coord;
    updateGeometry();
}

void Minimap::onResized(const sf::Vector2f &size)
{
    m_background.setSize(size);
    updateGeometry();
}

void Minimap::onThemeChanged(const WidgetTheme &widget_theme)
{
    m_background.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_background.setBorderSize(widget_theme.background.border_size);
    m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
    m_background.setBorderTextureSize(widget_theme.background.texture_border_size);

    m_rooms_map_grid.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_rooms_map_grid.setTextureRect({{128, 0}, {96, 192}});
    m_rooms_map_grid.setTextureCellSize({32, 32});
    m_rooms_map_grid.setCellSize({32, 32});

    m_room_map_grid_texture_indexes
        = {{0, 15},
           {room_side::LEFT, 0},
           {room_side::LEFT | room_side::TOP, 1},
           {room_side::LEFT | room_side::TOP | room_side::RIGHT, 2},
           {room_side::TOP, 3},
           {room_side::TOP | room_side::RIGHT, 4},
           {room_side::TOP | room_side::RIGHT | room_side::BOTTOM, 5},
           {room_side::RIGHT, 6},
           {room_side::RIGHT | room_side::BOTTOM, 7},
           {room_side::RIGHT | room_side::BOTTOM | room_side::LEFT, 8},
           {room_side::BOTTOM, 9},
           {room_side::BOTTOM | room_side::LEFT, 10},
           {room_side::BOTTOM | room_side::LEFT | room_side::TOP, 11},
           {room_side::TOP | room_side::BOTTOM, 12},
           {room_side::LEFT | room_side::RIGHT, 13},
           {room_side::LEFT | room_side::TOP | room_side::RIGHT | room_side::BOTTOM, 14}};

    m_rooms_type_grid.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_rooms_type_grid.setTextureRect({{128, 192}, {96, 32}});
    m_rooms_type_grid.setTextureCellSize({32, 32});
    m_rooms_type_grid.setCellSize({32, 32});

    m_room_type_grid_texture_indexes
        = {{room_type::UNKNOW, 0},
           {room_type::DEFAULT, -1},
           {room_type::BOSS, 1},
           {room_type::TRADER, 2}};

    m_current_room_highlight.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_current_room_highlight.setBorderSize(0.0f);
    m_current_room_highlight.setTextureRect({{160, 160}, {32, 32}});
    m_current_room_highlight.setSize({32, 32});
    m_current_room_highlight.setBorderTextureSize(0);

    updateGeometry();
}

void Minimap::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= getTransform();

    target.draw(m_background, new_states);

    {
        const WidgetTheme &widget_theme = getWidgetTheme();
        Clipping clipping(
            target,
            new_states,
            {widget_theme.padding.left, widget_theme.padding.top},
            getLocalBounds().getSize()
                - sf::Vector2f{
                    widget_theme.padding.left + widget_theme.padding.right,
                    widget_theme.padding.top + widget_theme.padding.bottom});

        target.draw(m_rooms_map_grid, new_states);
        target.draw(m_rooms_type_grid, new_states);
    }

    target.draw(m_current_room_highlight, new_states);
}

void Minimap::updateGeometry()
{
    sf::Vector2f position = getSize() / 2.0f
        - (vector2::mult(sf::Vector2f{m_current_room}, m_rooms_map_grid.getCellSize())
           + m_rooms_map_grid.getCellSize() / 2.0f);

    m_rooms_map_grid.setPoisition(position);
    m_rooms_type_grid.setPoisition(position);

    m_current_room_highlight.setPoisition((getSize() - m_current_room_highlight.getSize()) / 2.0f);
}

} // namespace fck::gui
