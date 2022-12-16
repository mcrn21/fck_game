#ifndef MINIMAP_HKXYERYPBHDA_H
#define MINIMAP_HKXYERYPBHDA_H

#include "frame.h"

#include "../../level.h"

#include <SFML/Graphics.hpp>

#include <unordered_map>

namespace fck::gui
{

class Minimap : public Frame
{
public:
    Minimap();
    Minimap(
        sf::Texture &texture,
        const sf::IntRect &frame_texture_rect,
        const std::unordered_map<int32_t, sf::IntRect> &room_texture_rects,
        const std::unordered_map<Room::Type, sf::IntRect> &room_type_texture_rects,
        const sf::IntRect &current_room_texture_rect);
    ~Minimap() = default;

    const Vector2D<Room *> *getRoomsMap() const;
    void setRoomsMap(const Vector2D<Room *> &rooms_map);

    const std::unordered_map<int32_t, sf::IntRect> &roomTextureRects() const;
    void setRoomTextureRects(const std::unordered_map<int32_t, sf::IntRect> &room_texture_rects);

    const std::unordered_map<Room::Type, sf::IntRect> &roomTypeTextureRects() const;
    void setRoomTypeTextureRects(
        const std::unordered_map<Room::Type, sf::IntRect> &room_type_texture_rects);

    const sf::IntRect &currentRoomTextureRect() const;
    void setCurrentRoomTextureRect(const sf::IntRect &current_room_texture_rect);

    sf::Text &text();

    void setRoomOpened(const sf::Vector2i &room_coord);
    void setCurrentRoom(const sf::Vector2i &room_coord);

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateRoomsPositions();
    void updateRoomsTexCoords();
    void updateRoomTexCoords(int32_t index);
    void updateRoomTypeTexCoords(int32_t index);

    void updateCurrentRoomPositions();
    void updateCurrentRoomTexCoords();

private:
    const Vector2D<Room *> *m_rooms_map;

    sf::Vector2f m_rooms_vertices_offset;
    sf::VertexArray m_rooms_vertices;
    std::unordered_map<int32_t, sf::IntRect> m_room_texture_rects;
    sf::VertexArray m_room_types_vertices;
    std::unordered_map<Room::Type, sf::IntRect> m_room_type_texture_rects;

    sf::Vector2f m_current_room_offset;
    sf::Vertex m_current_room_vertices[4];
    sf::IntRect m_current_room_texture_rect;

    sf::Text m_text;
};

} // namespace fck::gui

#endif // MINIMAP_H
