#include "minimap.h"

#include "../../fck/clipping.h"
#include "../../fck/utilities.h"

namespace fck::gui
{

Minimap::Minimap()
{
}

sf::Texture *Minimap::texture() const
{
    return m_texture;
}

void Minimap::setTexture(sf::Texture &texture)
{
    m_texture = &texture;
}

const sf::IntRect &Minimap::backgroundTextureRect() const
{
    return m_background_texture_rect;
}

void Minimap::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        new_state.transform *= getTransform();
        target.draw(m_background_vertices, 4, sf::TriangleStrip, new_state);

        {
            sf::RenderStates rooms_state = new_state;
            Clipping clipping(
                target,
                rooms_state,
                {2.0f, 2.0f},
                localBounds().getSize() - sf::Vector2f{4.0f, 4.0f});

            rooms_state.transform.translate(m_rooms_vertices_offset);
            target.draw(m_rooms_vertices, rooms_state);
            target.draw(m_room_types_vertices, rooms_state);
        }

        sf::RenderStates current_room_state = new_state;
        current_room_state.transform.translate(m_current_room_offset);
        target.draw(m_current_room_vertices, 4, sf::TriangleStrip, current_room_state);

        target.draw(m_text, new_state);
    }
}

void Minimap::setBackgroundTextureRect(const sf::IntRect &background_texture_rect)
{
    m_background_texture_rect = background_texture_rect;
    updateBackgroundPositions();
    updateBackgroundTexCoords();
}

const std::unordered_map<int32_t, sf::IntRect> &Minimap::roomTextureRects() const
{
    return m_room_texture_rects;
}

void Minimap::setRoomTextureRects(
    const std::unordered_map<int32_t, sf::IntRect> &room_texture_rects)
{
    m_room_texture_rects = room_texture_rects;
}

const sf::IntRect &Minimap::roomUnopenedTextureRect() const
{
    return m_room_unopened_texture_rect;
}

void Minimap::setRoomUnopenedTextureRect(const sf::IntRect &room_unopened_texture_rect)
{
    m_room_unopened_texture_rect = room_unopened_texture_rect;
}

const std::unordered_map<Level::Room::Type, sf::IntRect> &Minimap::roomTypeTextureRects() const
{
    return m_room_type_texture_rects;
}

void Minimap::setRoomTypeTextureRects(
    const std::unordered_map<Level::Room::Type, sf::IntRect> &room_type_texture_rects)
{
    m_room_type_texture_rects = room_type_texture_rects;
}

const sf::IntRect &Minimap::currentRoomTextureRect() const
{
    return m_current_room_texture_rect;
}

void Minimap::setCurrentRoomTextureRect(const sf::IntRect &current_room_texture_rect)
{
    m_current_room_texture_rect = current_room_texture_rect;

    updateCurrentRoomPositions();
    updateCurrentRoomTexCoords();

    m_current_room_offset
        = {float(m_background_texture_rect.width) / 2.0f
               - float(m_current_room_texture_rect.width) / 2.0f,
           float(m_background_texture_rect.height) / 2.0f
               - float(m_current_room_texture_rect.height) / 2.0f};
}

sf::Text &Minimap::text()
{
    return m_text;
}

void Minimap::updateRooms(const Vector2D<Level::Room *> &rooms_map)
{
    m_rooms_vertices.clear();
    m_room_types_vertices.clear();

    if (m_room_texture_rects.empty())
        return;

    sf::Vector2i room_size = m_room_texture_rects.begin()->second.getSize();

    m_rooms_vertices.setPrimitiveType(sf::Triangles);
    m_rooms_vertices.resize(rooms_map.size() * 6);

    m_room_types_vertices.setPrimitiveType(sf::Triangles);
    m_room_types_vertices.resize(rooms_map.size() * 6);

    for (int32_t i = 0; i < rooms_map.size(); ++i)
    {
        sf::Vector2i coord = rooms_map.transformIndex(i);

        sf::Vertex *room_quad = &m_rooms_vertices[i * 6];

        room_quad[0].position = sf::Vector2f(coord.x * room_size.x, coord.y * room_size.y);
        room_quad[1].position = sf::Vector2f((coord.x + 1) * room_size.x, coord.y * room_size.y);
        room_quad[2].position = sf::Vector2f(coord.x * room_size.x, (coord.y + 1) * room_size.y);
        room_quad[3].position = sf::Vector2f((coord.x + 1) * room_size.x, coord.y * room_size.y);
        room_quad[4].position = sf::Vector2f(coord.x * room_size.x, (coord.y + 1) * room_size.y);
        room_quad[5].position
            = sf::Vector2f((coord.x + 1) * room_size.x, (coord.y + 1) * room_size.y);

        if (rooms_map.at(i))
        {
            const sf::IntRect &room_texture_rect = m_room_texture_rects[rooms_map.at(i)->neighbors];

            float left = float(room_texture_rect.left);
            float right = left + room_texture_rect.width;
            float top = float(room_texture_rect.top);
            float bottom = top + room_texture_rect.height;

            room_quad[0].texCoords = sf::Vector2f(left, top);
            room_quad[1].texCoords = sf::Vector2f(right, top);
            room_quad[2].texCoords = sf::Vector2f(left, bottom);
            room_quad[3].texCoords = sf::Vector2f(right, top);
            room_quad[4].texCoords = sf::Vector2f(left, bottom);
            room_quad[5].texCoords = sf::Vector2f(right, bottom);
        }
        else
        {
            room_quad[0].texCoords = sf::Vector2f(0, 0);
            room_quad[1].texCoords = sf::Vector2f(0, 0);
            room_quad[2].texCoords = sf::Vector2f(0, 0);
            room_quad[3].texCoords = sf::Vector2f(0, 0);
            room_quad[4].texCoords = sf::Vector2f(0, 0);
            room_quad[5].texCoords = sf::Vector2f(0, 0);
        }

        sf::Vertex *room_type_quad = &m_room_types_vertices[i * 6];

        room_type_quad[0].position = sf::Vector2f(coord.x * room_size.x, coord.y * room_size.y);
        room_type_quad[1].position
            = sf::Vector2f((coord.x + 1) * room_size.x, coord.y * room_size.y);
        room_type_quad[2].position
            = sf::Vector2f(coord.x * room_size.x, (coord.y + 1) * room_size.y);
        room_type_quad[3].position
            = sf::Vector2f((coord.x + 1) * room_size.x, coord.y * room_size.y);
        room_type_quad[4].position
            = sf::Vector2f(coord.x * room_size.x, (coord.y + 1) * room_size.y);
        room_type_quad[5].position
            = sf::Vector2f((coord.x + 1) * room_size.x, (coord.y + 1) * room_size.y);

        if (rooms_map.at(i))
        {
            const sf::IntRect &room_type_texture_rect = rooms_map.at(i)->opened
                ? m_room_type_texture_rects[rooms_map.at(i)->type]
                : m_room_unopened_texture_rect;

            float left = float(room_type_texture_rect.left);
            float right = left + room_type_texture_rect.width;
            float top = float(room_type_texture_rect.top);
            float bottom = top + room_type_texture_rect.height;

            room_type_quad[0].texCoords = sf::Vector2f(left, top);
            room_type_quad[1].texCoords = sf::Vector2f(right, top);
            room_type_quad[2].texCoords = sf::Vector2f(left, bottom);
            room_type_quad[3].texCoords = sf::Vector2f(right, top);
            room_type_quad[4].texCoords = sf::Vector2f(left, bottom);
            room_type_quad[5].texCoords = sf::Vector2f(right, bottom);
        }
        else
        {
            room_type_quad[0].texCoords = sf::Vector2f(0, 0);
            room_type_quad[1].texCoords = sf::Vector2f(0, 0);
            room_type_quad[2].texCoords = sf::Vector2f(0, 0);
            room_type_quad[3].texCoords = sf::Vector2f(0, 0);
            room_type_quad[4].texCoords = sf::Vector2f(0, 0);
            room_type_quad[5].texCoords = sf::Vector2f(0, 0);
        }
    }
}

void Minimap::setCurrentRoom(const sf::Vector2i &room_coord)
{
    if (m_rooms_vertices.getVertexCount() == 0)
        return;
    sf::Vector2i room_size = m_room_texture_rects.begin()->second.getSize();
    m_rooms_vertices_offset
        = sf::Vector2f{m_background_texture_rect.getSize()} / 2.0f - sf::Vector2f(room_size) / 2.0f;
    m_rooms_vertices_offset -= sf::Vector2f{vector2::mult(room_size, room_coord)};
}

void Minimap::setRoomOpened(
    const Vector2D<Level::Room *> &rooms_map, const sf::Vector2i &room_coord)
{
    if (m_room_types_vertices.getVertexCount() == 0)
        return;

    int32_t index = room_coord.y * rooms_map.size2D().x + room_coord.x;

    sf::Vertex *room_type_quad = &m_room_types_vertices[index * 6];

    const sf::IntRect &room_type_texture_rect = rooms_map.at(index)->opened
        ? m_room_type_texture_rects[rooms_map.at(index)->type]
        : m_room_unopened_texture_rect;

    float left = float(room_type_texture_rect.left);
    float right = left + room_type_texture_rect.width;
    float top = float(room_type_texture_rect.top);
    float bottom = top + room_type_texture_rect.height;

    room_type_quad[0].texCoords = sf::Vector2f(left, top);
    room_type_quad[1].texCoords = sf::Vector2f(right, top);
    room_type_quad[2].texCoords = sf::Vector2f(left, bottom);
    room_type_quad[3].texCoords = sf::Vector2f(right, top);
    room_type_quad[4].texCoords = sf::Vector2f(left, bottom);
    room_type_quad[5].texCoords = sf::Vector2f(right, bottom);
}

sf::FloatRect Minimap::localBounds() const
{
    auto width = static_cast<float>(std::abs(m_background_texture_rect.width));
    auto height = static_cast<float>(std::abs(m_background_texture_rect.height));

    return sf::FloatRect({0.f, 0.f}, {width, height});
}

sf::FloatRect Minimap::globalBounds() const
{
    return getTransform().transformRect(localBounds());
}

void Minimap::updateBackgroundPositions()
{
    m_background_vertices[0].position = sf::Vector2f(0, 0);
    m_background_vertices[1].position = sf::Vector2f(0, m_background_texture_rect.height);
    m_background_vertices[2].position = sf::Vector2f(m_background_texture_rect.width, 0);
    m_background_vertices[3].position
        = sf::Vector2f(m_background_texture_rect.width, m_background_texture_rect.height);
}

void Minimap::updateBackgroundTexCoords()
{
    float left = static_cast<float>(m_background_texture_rect.left);
    float right = left + m_background_texture_rect.width;
    float top = static_cast<float>(m_background_texture_rect.top);
    float bottom = top + m_background_texture_rect.height;

    m_background_vertices[0].texCoords = sf::Vector2f(left, top);
    m_background_vertices[1].texCoords = sf::Vector2f(left, bottom);
    m_background_vertices[2].texCoords = sf::Vector2f(right, top);
    m_background_vertices[3].texCoords = sf::Vector2f(right, bottom);
}

void Minimap::updateCurrentRoomPositions()
{
    m_current_room_vertices[0].position = sf::Vector2f(0, 0);
    m_current_room_vertices[1].position = sf::Vector2f(0, m_current_room_texture_rect.height);
    m_current_room_vertices[2].position = sf::Vector2f(m_current_room_texture_rect.width, 0);
    m_current_room_vertices[3].position
        = sf::Vector2f(m_current_room_texture_rect.width, m_current_room_texture_rect.height);
}

void Minimap::updateCurrentRoomTexCoords()
{
    float left = static_cast<float>(m_current_room_texture_rect.left);
    float right = left + m_current_room_texture_rect.width;
    float top = static_cast<float>(m_current_room_texture_rect.top);
    float bottom = top + m_current_room_texture_rect.height;

    m_current_room_vertices[0].texCoords = sf::Vector2f(left, top);
    m_current_room_vertices[1].texCoords = sf::Vector2f(left, bottom);
    m_current_room_vertices[2].texCoords = sf::Vector2f(right, top);
    m_current_room_vertices[3].texCoords = sf::Vector2f(right, bottom);
}

} // namespace fck::gui
