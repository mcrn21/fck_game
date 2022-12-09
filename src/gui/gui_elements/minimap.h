#ifndef MINIMAP_HKXYERYPBHDA_H
#define MINIMAP_HKXYERYPBHDA_H

#include "../../level.h"

#include <SFML/Graphics.hpp>

#include <unordered_map>

namespace fck::gui
{

class Minimap : public sf::Drawable, public sf::Transformable
{
public:
    Minimap();
    ~Minimap() = default;

    sf::Texture *texture() const;
    void setTexture(sf::Texture &texture);

    const sf::IntRect &backgroundTextureRect() const;
    void setBackgroundTextureRect(const sf::IntRect &background_texture_rect);

    const std::unordered_map<int32_t, sf::IntRect> &roomTextureRects() const;
    void setRoomTextureRects(const std::unordered_map<int32_t, sf::IntRect> &room_texture_rects);

    const sf::IntRect &roomUnopenedTextureRect() const;
    void setRoomUnopenedTextureRect(const sf::IntRect &room_unopened_texture_rect);

    const std::unordered_map<Level::Room::Type, sf::IntRect> &roomTypeTextureRects() const;
    void setRoomTypeTextureRects(
        const std::unordered_map<Level::Room::Type, sf::IntRect> &room_type_texture_rects);

    const sf::IntRect &currentRoomTextureRect() const;
    void setCurrentRoomTextureRect(const sf::IntRect &current_room_texture_rect);

    sf::Text &text();

    void updateRooms(const Vector2D<Level::Room *> &rooms_map);
    void setCurrentRoom(const sf::Vector2i &room_coord);
    void setRoomOpened(const Vector2D<Level::Room *> &rooms_map, const sf::Vector2i &room_coord);

    sf::FloatRect localBounds() const;
    sf::FloatRect globalBounds() const;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateBackgroundPositions();
    void updateBackgroundTexCoords();

    void updateCurrentRoomPositions();
    void updateCurrentRoomTexCoords();

private:
    sf::Texture *m_texture;

    sf::Vertex m_background_vertices[4];
    sf::IntRect m_background_texture_rect;

    sf::Vector2f m_rooms_vertices_offset;
    sf::VertexArray m_rooms_vertices;
    std::unordered_map<int32_t, sf::IntRect> m_room_texture_rects;

    sf::VertexArray m_room_types_vertices;
    sf::IntRect m_room_unopened_texture_rect;
    std::unordered_map<Level::Room::Type, sf::IntRect> m_room_type_texture_rects;

    sf::Vector2f m_current_room_offset;
    sf::Vertex m_current_room_vertices[4];
    sf::IntRect m_current_room_texture_rect;

    sf::Text m_text;
};

} // namespace fck::gui

#endif // MINIMAP_H
