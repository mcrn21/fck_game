#include "minimap.h"

#include "../../fck/clipping.h"
#include "../../fck/utilities.h"

namespace fck::gui
{

Minimap::Minimap() : Frame{}
{
}

Minimap::Minimap(
    sf::Texture &texture,
    const sf::IntRect &frame_texture_rect,
    const std::unordered_map<int32_t, sf::IntRect> &room_texture_rects,
    const std::unordered_map<room_type::Type, sf::IntRect> &room_type_texture_rects,
    const sf::IntRect &current_room_texture_rect)
    : Frame{texture, frame_texture_rect},
      m_room_texture_rects{room_texture_rects},
      m_room_type_texture_rects{room_type_texture_rects},
      m_current_room_texture_rect{current_room_texture_rect}
{
    updateCurrentRoomPositions();
    updateCurrentRoomTexCoords();
}

const Vector2D<Room *> *Minimap::getRoomsMap() const
{
    return m_rooms_map;
}

void Minimap::setRoomsMap(const Vector2D<Room *> &rooms_map)
{
    m_rooms_map = &rooms_map;

    updateRoomsPositions();
    updateRoomsTexCoords();
}

void Minimap::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    Frame::draw(target, states);

    if (getTexture())
    {
        {
            sf::RenderStates rooms_state = states;
            rooms_state.texture = getTexture();
            rooms_state.transform *= getParentsTransform();

            Clipping clipping(
                target,
                rooms_state,
                {2.0f, 2.0f},
                getLocalBounds().getSize() - sf::Vector2f{4.0f, 5.0f});

            rooms_state.transform.translate(m_rooms_vertices_offset);
            target.draw(m_rooms_vertices, rooms_state);
            target.draw(m_room_types_vertices, rooms_state);
        }

        sf::RenderStates current_room_state = states;
        current_room_state.texture = getTexture();
        current_room_state.transform *= getParentsTransform();
        current_room_state.transform.translate(m_current_room_offset);
        target.draw(m_current_room_vertices, 4, sf::TriangleStrip, current_room_state);

        sf::RenderStates text_state = states;
        text_state.transform *= getParentsTransform();
        target.draw(m_text, text_state);
    }
}

const std::unordered_map<int32_t, sf::IntRect> &Minimap::roomTextureRects() const
{
    return m_room_texture_rects;
}

void Minimap::setRoomTextureRects(
    const std::unordered_map<int32_t, sf::IntRect> &room_texture_rects)
{
    m_room_texture_rects = room_texture_rects;

    updateRoomsPositions();
    updateRoomsTexCoords();
}

const std::unordered_map<room_type::Type, sf::IntRect> &Minimap::roomTypeTextureRects() const
{
    return m_room_type_texture_rects;
}

void Minimap::setRoomTypeTextureRects(
    const std::unordered_map<room_type::Type, sf::IntRect> &room_type_texture_rects)
{
    m_room_type_texture_rects = room_type_texture_rects;

    updateRoomsPositions();
    updateRoomsTexCoords();
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
}

sf::Text &Minimap::text()
{
    return m_text;
}

void Minimap::setRoomOpened(const sf::Vector2i &room_coord)
{
    if (!m_rooms_map || m_room_texture_rects.empty() || m_room_type_texture_rects.empty())
        return;

    int32_t index = room_coord.y * m_rooms_map->getSize2D().x + room_coord.x;
    updateRoomTypeTexCoords(index);
}

void Minimap::setCurrentRoom(const sf::Vector2i &room_coord)
{
    if (!m_rooms_map || m_room_texture_rects.empty() || m_room_type_texture_rects.empty())
        return;

    sf::Vector2i room_size = m_room_texture_rects.begin()->second.getSize();
    m_rooms_vertices_offset = getLocalBounds().getSize() / 2.0f - sf::Vector2f(room_size) / 2.0f;
    m_rooms_vertices_offset -= sf::Vector2f{vector2::mult(room_size, room_coord)};

    m_current_room_offset = getLocalBounds().getSize() / 2.0f
        - sf::Vector2f{m_current_room_texture_rect.getSize()} / 2.0f;
}

void Minimap::updateRoomsPositions()
{
    m_rooms_vertices.clear();
    m_room_types_vertices.clear();

    if (!m_rooms_map || m_room_texture_rects.empty() || m_room_type_texture_rects.empty())
        return;

    sf::Vector2i room_size = m_room_texture_rects.begin()->second.getSize();

    m_rooms_vertices.setPrimitiveType(sf::Triangles);
    m_rooms_vertices.resize(m_rooms_map->getSize() * 6);

    m_room_types_vertices.setPrimitiveType(sf::Triangles);
    m_room_types_vertices.resize(m_rooms_map->getSize() * 6);

    for (int32_t i = 0; i < m_rooms_map->getSize(); ++i)
    {
        sf::Vector2i coord = m_rooms_map->transformIndex(i);

        sf::Vertex *room_quad = &m_rooms_vertices[i * 6];
        room_quad[0].position = sf::Vector2f(coord.x * room_size.x, coord.y * room_size.y);
        room_quad[1].position = sf::Vector2f((coord.x + 1) * room_size.x, coord.y * room_size.y);
        room_quad[2].position = sf::Vector2f(coord.x * room_size.x, (coord.y + 1) * room_size.y);
        room_quad[3].position = sf::Vector2f((coord.x + 1) * room_size.x, coord.y * room_size.y);
        room_quad[4].position = sf::Vector2f(coord.x * room_size.x, (coord.y + 1) * room_size.y);
        room_quad[5].position
            = sf::Vector2f((coord.x + 1) * room_size.x, (coord.y + 1) * room_size.y);

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
    }
}

void Minimap::updateRoomsTexCoords()
{
    if (!m_rooms_map || m_room_texture_rects.empty() || m_room_type_texture_rects.empty())
        return;

    for (int32_t i = 0; i < m_rooms_map->getSize(); ++i)
    {
        if (m_rooms_map->at(i))
        {
            updateRoomTexCoords(i);
            updateRoomTypeTexCoords(i);
        }
    }
}

void Minimap::updateRoomTexCoords(int32_t index)
{
    sf::Vertex *room_quad = &m_rooms_vertices[index * 6];

    const sf::IntRect &room_texture_rect
        = m_room_texture_rects[m_rooms_map->at(index)->getNeighbors()];

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

void Minimap::updateRoomTypeTexCoords(int32_t index)
{
    sf::Vertex *room_type_quad = &m_room_types_vertices[index * 6];

    const sf::IntRect &room_type_texture_rect = m_rooms_map->at(index)->isOpen()
        ? m_room_type_texture_rects[m_rooms_map->at(index)->getType()]
        : m_room_type_texture_rects[room_type::UNKNOW];

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
