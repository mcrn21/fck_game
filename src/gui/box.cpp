#include "box.h"

namespace fck::gui
{

Box::Box() : m_texture{nullptr}
{
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(54);
}

const sf::Texture *Box::getTexture() const
{
    return m_texture;
}

void Box::setTexture(const sf::Texture &texture)
{
    m_texture = &texture;
}

const sf::Vector2f &Box::getPosition() const
{
    return m_position;
}

void Box::setPoisition(const sf::Vector2f &position)
{
    m_position = position;
    updatePositions();
}

const sf::Vector2f &Box::getSize() const
{
    return m_size;
}

void Box::setSize(const sf::Vector2f &size)
{
    m_size = size;
    updatePositions();
}

const Sides<float> &Box::getBorderSize() const
{
    return m_border_size;
}

void Box::setBorderSize(const Sides<float> &border_size)
{
    m_border_size = border_size;
    updatePositions();
}

const sf::Color &Box::getColor() const
{
    return m_vertices[0].color;
}

void Box::setColor(const sf::Color &color)
{
    for (int32_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = color;
}

const sf::IntRect &Box::getTextureRect() const
{
    return m_texture_rect;
}

void Box::setTextureRect(const sf::IntRect &frame_texture_rect)
{
    m_texture_rect = frame_texture_rect;
    updateTexCoords();
}

const Sides<int32_t> &Box::getBorderTextureSize() const
{
    return m_border_texture_size;
}

void Box::setBorderTextureSize(const Sides<int32_t> &border_texture_size)
{
    m_border_texture_size = border_texture_size;
    updateTexCoords();
}

bool Box::containsPoint(const sf::Vector2f &point) const
{
    return point.x >= m_position.x && point.x < (m_position.x + m_size.x) && point.y >= m_position.y
        && point.y < (m_position.y + m_size.y);
}

void Box::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        target.draw(m_vertices, new_state);
    }
}

void Box::updatePositions()
{
    sf::Vector2f size;
    size.x = std::max(0.0f, m_size.x - (m_border_size.left + m_border_size.right));
    size.y = std::max(0.0f, m_size.y - (m_border_size.top + m_border_size.bottom));

    // left top
    m_vertices[0].position = m_position + sf::Vector2f{0.0f, 0.0f};
    m_vertices[1].position = m_position + sf::Vector2f{m_border_size.left, 0.0f};
    m_vertices[2].position = m_position + sf::Vector2f{0.0f, m_border_size.top};
    m_vertices[3].position = m_position + sf::Vector2f{m_border_size.left, 0.0f};
    m_vertices[4].position = m_position + sf::Vector2f{0.0f, m_border_size.top};
    m_vertices[5].position = m_position + sf::Vector2f{m_border_size.left, m_border_size.top};

    // top
    m_vertices[6].position = m_position + sf::Vector2f{m_border_size.left, 0.0f};
    m_vertices[7].position = m_position + sf::Vector2f{size.x + m_border_size.left, 0.0f};
    m_vertices[8].position = m_position + sf::Vector2f{m_border_size.left, m_border_size.top};
    m_vertices[9].position = m_position + sf::Vector2f{size.x + m_border_size.left, 0.0f};
    m_vertices[10].position = m_position + sf::Vector2f{m_border_size.left, m_border_size.top};
    m_vertices[11].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, m_border_size.top};

    // top right
    m_vertices[12].position = m_position + sf::Vector2f{size.x + m_border_size.left, 0.0f};
    m_vertices[13].position
        = m_position + sf::Vector2f{size.x + (m_border_size.left + m_border_size.right), 0.0f};
    m_vertices[14].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, m_border_size.top};
    m_vertices[15].position
        = m_position + sf::Vector2f{size.x + (m_border_size.left + m_border_size.right), 0.0f};
    m_vertices[16].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, m_border_size.top};
    m_vertices[17].position = m_position
        + sf::Vector2f{size.x + (m_border_size.left + m_border_size.right), m_border_size.top};

    // left
    m_vertices[18].position = m_position + sf::Vector2f{0.0f, m_border_size.top};
    m_vertices[19].position = m_position + sf::Vector2f{m_border_size.left, m_border_size.top};
    m_vertices[20].position = m_position + sf::Vector2f{0.0f, size.y + m_border_size.top};
    m_vertices[21].position = m_position + sf::Vector2f{m_border_size.left, m_border_size.top};
    m_vertices[22].position = m_position + sf::Vector2f{0.0f, size.y + m_border_size.top};
    m_vertices[23].position
        = m_position + sf::Vector2f{m_border_size.left, size.y + m_border_size.top};

    // center
    m_vertices[24].position = m_position + sf::Vector2f{m_border_size.left, m_border_size.top};
    m_vertices[25].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, m_border_size.top};
    m_vertices[26].position
        = m_position + sf::Vector2f{m_border_size.left, size.y + m_border_size.top};
    m_vertices[27].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, m_border_size.top};
    m_vertices[28].position
        = m_position + sf::Vector2f{m_border_size.left, size.y + m_border_size.top};
    m_vertices[29].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, size.y + m_border_size.top};

    // right
    m_vertices[30].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, m_border_size.top};
    m_vertices[31].position = m_position
        + sf::Vector2f{size.x + (m_border_size.left + m_border_size.right), m_border_size.top};
    m_vertices[32].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, size.y + m_border_size.top};
    m_vertices[33].position = m_position
        + sf::Vector2f{size.x + (m_border_size.left + m_border_size.right), m_border_size.top};
    m_vertices[34].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, size.y + m_border_size.top};
    m_vertices[35].position = m_position
        + sf::Vector2f{
            size.x + (m_border_size.left + m_border_size.right), size.y + m_border_size.top};

    // left bottom
    m_vertices[36].position = m_position + sf::Vector2f{0.0f, size.y + m_border_size.top};
    m_vertices[37].position
        = m_position + sf::Vector2f{m_border_size.left, size.y + m_border_size.top};
    m_vertices[38].position
        = m_position + sf::Vector2f{0.0f, size.y + (m_border_size.top + m_border_size.bottom)};
    m_vertices[39].position
        = m_position + sf::Vector2f{m_border_size.left, size.y + m_border_size.top};
    m_vertices[40].position
        = m_position + sf::Vector2f{0.0f, size.y + (m_border_size.top + m_border_size.bottom)};
    m_vertices[41].position = m_position
        + sf::Vector2f{m_border_size.left, size.y + (m_border_size.top + m_border_size.bottom)};

    // bottom
    m_vertices[42].position
        = m_position + sf::Vector2f{m_border_size.left, size.y + m_border_size.top};
    m_vertices[43].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, size.y + m_border_size.top};
    m_vertices[44].position = m_position
        + sf::Vector2f{m_border_size.left, size.y + (m_border_size.top + m_border_size.bottom)};
    m_vertices[45].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, size.y + m_border_size.top};
    m_vertices[46].position = m_position
        + sf::Vector2f{m_border_size.left, size.y + (m_border_size.top + m_border_size.bottom)};
    m_vertices[47].position = m_position
        + sf::Vector2f{
            size.x + m_border_size.left, size.y + (m_border_size.top + m_border_size.bottom)};

    // bottom right
    m_vertices[48].position
        = m_position + sf::Vector2f{size.x + m_border_size.left, size.y + m_border_size.top};
    m_vertices[49].position = m_position
        + sf::Vector2f{
            size.x + (m_border_size.left + m_border_size.right), size.y + m_border_size.top};
    m_vertices[50].position = m_position
        + sf::Vector2f{
            size.x + m_border_size.left, size.y + (m_border_size.top + m_border_size.bottom)};
    m_vertices[51].position = m_position
        + sf::Vector2f{
            size.x + (m_border_size.left + m_border_size.right), size.y + m_border_size.top};
    m_vertices[52].position = m_position
        + sf::Vector2f{
            size.x + m_border_size.left, size.y + (m_border_size.top + m_border_size.bottom)};
    m_vertices[53].position = m_position
        + sf::Vector2f{
            size.x + (m_border_size.left + m_border_size.right),
            size.y + (m_border_size.top + m_border_size.bottom)};
}

void Box::updateTexCoords()
{
    float left = static_cast<float>(m_texture_rect.left);
    float top = static_cast<float>(m_texture_rect.top);

    sf::Vector2f size;
    size.x = std::max(
        0.0f,
        float(m_texture_rect.width - (m_border_texture_size.left + m_border_texture_size.right)));
    size.y = std::max(
        0.0f,
        float(m_texture_rect.height - (m_border_texture_size.top + m_border_texture_size.bottom)));

    if (size.x == 0 || size.y == 0)
    {
        for (int32_t i = 0; i < 54; ++i)
            m_vertices[i].texCoords = {0.0f, 0.0f};
        return;
    }

    // left top
    m_vertices[0].texCoords = {left, top};
    m_vertices[1].texCoords = {left + m_border_texture_size.left, top};
    m_vertices[2].texCoords = {left, top + m_border_texture_size.top};
    m_vertices[3].texCoords = {left + m_border_texture_size.left, top};
    m_vertices[4].texCoords = {left, top + m_border_texture_size.top};
    m_vertices[5].texCoords = {left + m_border_texture_size.left, top + m_border_texture_size.top};

    // top
    m_vertices[6].texCoords = {left + m_border_texture_size.left, top};
    m_vertices[7].texCoords = {size.x + left + m_border_texture_size.left, top};
    m_vertices[8].texCoords = {left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[9].texCoords = {size.x + left + m_border_texture_size.left, top};
    m_vertices[10].texCoords = {left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[11].texCoords
        = {size.x + left + m_border_texture_size.left, top + m_border_texture_size.top};

    // top right
    m_vertices[12].texCoords = {size.x + left + m_border_texture_size.left, top};
    m_vertices[13].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right), top};
    m_vertices[14].texCoords
        = {size.x + left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[15].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right), top};
    m_vertices[16].texCoords
        = {size.x + left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[17].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right),
           top + m_border_texture_size.top};

    // left
    m_vertices[18].texCoords = {left, top + m_border_texture_size.top};
    m_vertices[19].texCoords = {left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[20].texCoords = {left, size.y + top + m_border_texture_size.top};
    m_vertices[21].texCoords = {left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[22].texCoords = {left, size.y + top + m_border_texture_size.top};
    m_vertices[23].texCoords
        = {left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};

    // center
    m_vertices[24].texCoords = {left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[25].texCoords
        = {size.x + left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[26].texCoords
        = {left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[27].texCoords
        = {size.x + left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[28].texCoords
        = {left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[29].texCoords
        = {size.x + left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};

    // right
    m_vertices[30].texCoords
        = {size.x + left + m_border_texture_size.left, top + m_border_texture_size.top};
    m_vertices[31].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right),
           top + m_border_texture_size.top};
    m_vertices[32].texCoords
        = {size.x + left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[33].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right),
           top + m_border_texture_size.top};
    m_vertices[34].texCoords
        = {size.x + left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[35].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right),
           size.y + top + m_border_texture_size.top};

    // left bottom
    m_vertices[36].texCoords = {left, size.y + top + m_border_texture_size.top};
    m_vertices[37].texCoords
        = {left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[38].texCoords
        = {left, size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};
    m_vertices[39].texCoords
        = {left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[40].texCoords
        = {left, size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};
    m_vertices[41].texCoords
        = {left + m_border_texture_size.left,
           size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};

    // bottom
    m_vertices[42].texCoords
        = {left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[43].texCoords
        = {size.x + left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[44].texCoords
        = {left + m_border_texture_size.left,
           size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};
    m_vertices[45].texCoords
        = {size.x + left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[46].texCoords
        = {left + m_border_texture_size.left,
           size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};
    m_vertices[47].texCoords
        = {size.x + left + m_border_texture_size.left,
           size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};

    // bottom right
    m_vertices[48].texCoords
        = {size.x + left + m_border_texture_size.left, size.y + top + m_border_texture_size.top};
    m_vertices[49].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right),
           size.y + top + m_border_texture_size.top};
    m_vertices[50].texCoords
        = {size.x + left + m_border_texture_size.left,
           size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};
    m_vertices[51].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right),
           size.y + top + m_border_texture_size.top};
    m_vertices[52].texCoords
        = {size.x + left + m_border_texture_size.left,
           size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};
    m_vertices[53].texCoords
        = {size.x + left + (m_border_texture_size.left + m_border_texture_size.right),
           size.y + top + (m_border_texture_size.top + m_border_texture_size.bottom)};
}

} // namespace fck::gui
