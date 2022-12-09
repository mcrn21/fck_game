#include "frame.h"

namespace fck::gui
{

Frame::Frame() : m_texture{nullptr}
{
}

Frame::Frame(sf::Texture &texture, const sf::IntRect &frame_texture_rect)
    : m_texture{&texture}, m_frame_texture_rect{frame_texture_rect}
{
    updatePositions();
    updateTexCoords();
}

sf::Texture *Frame::getTexture() const
{
    return m_texture;
}

void Frame::setTexture(sf::Texture &texture)
{
    m_texture = &texture;
}

const sf::Vector2f &Frame::getFrameSize() const
{
    return m_frame_size;
}

void Frame::setFrameSize(const sf::Vector2f &frame_size)
{
    m_frame_size = frame_size;
    updatePositions();
}

const sf::Vector2i &Frame::getBorderSize() const
{
    return m_border_size;
}

void Frame::setBorderSize(const sf::Vector2i &border_size)
{
    m_border_size = border_size;
    updatePositions();
    updateTexCoords();
}

const sf::IntRect &Frame::getFrameTextureRect() const
{
    return m_frame_texture_rect;
}

void Frame::setFrameTextureRect(const sf::IntRect &frame_texture_rect)
{
    m_frame_texture_rect = frame_texture_rect;
    updatePositions();
    updateTexCoords();
}

sf::FloatRect Frame::getLocalBounds() const
{
    return m_vertices.getBounds();
}

sf::FloatRect Frame::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

void Frame::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        new_state.transform *= getTransform();
        target.draw(m_vertices, new_state);
    }
}

void Frame::updatePositions()
{
    if (!m_texture)
        return;

    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(54);

    sf::Vector2f size;
    size.x = std::max(0.0f, float(m_frame_size.x - m_border_size.x * 2));
    size.y = std::max(0.0f, float(m_frame_size.y - m_border_size.y * 2));

    // left top
    m_vertices[0].position = {0.0f, 0.0f};
    m_vertices[1].position = {float(m_border_size.x), 0.0f};
    m_vertices[2].position = {0.0f, float(m_border_size.y)};
    m_vertices[3].position = {float(m_border_size.x), 0.0f};
    m_vertices[4].position = {0.0f, float(m_border_size.y)};
    m_vertices[5].position = {float(m_border_size.x), float(m_border_size.y)};

    // top
    m_vertices[6].position = {float(m_border_size.x), 0.0f};
    m_vertices[7].position = {size.x + float(m_border_size.x), 0.0f};
    m_vertices[8].position = {float(m_border_size.x), float(m_border_size.y)};
    m_vertices[9].position = {size.x + float(m_border_size.x), 0.0f};
    m_vertices[10].position = {float(m_border_size.x), float(m_border_size.y)};
    m_vertices[11].position = {size.x + float(m_border_size.x), float(m_border_size.y)};

    // top right
    m_vertices[12].position = {size.x + float(m_border_size.x), 0.0f};
    m_vertices[13].position = {size.x + float(m_border_size.x * 2), 0.0f};
    m_vertices[14].position = {size.x + float(m_border_size.x), float(m_border_size.y)};
    m_vertices[15].position = {size.x + float(m_border_size.x * 2), 0.0f};
    m_vertices[16].position = {size.x + float(m_border_size.x), float(m_border_size.y)};
    m_vertices[17].position = {size.x + float(m_border_size.x * 2), float(m_border_size.y)};

    // left
    m_vertices[18].position = {0.0f, float(m_border_size.y)};
    m_vertices[19].position = {float(m_border_size.x), float(m_border_size.y)};
    m_vertices[20].position = {0.0f, size.y + float(m_border_size.y)};
    m_vertices[21].position = {float(m_border_size.x), float(m_border_size.y)};
    m_vertices[22].position = {0.0f, size.y + float(m_border_size.y)};
    m_vertices[23].position = {float(m_border_size.x), size.y + float(m_border_size.y)};

    // center
    m_vertices[24].position = {float(m_border_size.x), float(m_border_size.y)};
    m_vertices[25].position = {size.x + float(m_border_size.x), float(m_border_size.y)};
    m_vertices[26].position = {float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[27].position = {size.x + float(m_border_size.x), float(m_border_size.y)};
    m_vertices[28].position = {float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[29].position = {size.x + float(m_border_size.x), size.y + float(m_border_size.y)};

    // right
    m_vertices[30].position = {size.x + float(m_border_size.x), float(m_border_size.y)};
    m_vertices[31].position = {size.x + float(m_border_size.x * 2), float(m_border_size.y)};
    m_vertices[32].position = {size.x + float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[33].position = {size.x + float(m_border_size.x * 2), float(m_border_size.y)};
    m_vertices[34].position = {size.x + float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[35].position
        = {size.x + float(m_border_size.x * 2), size.y + float(m_border_size.y)};

    // left bottom
    m_vertices[36].position = {0.0f, size.y + float(m_border_size.y)};
    m_vertices[37].position = {float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[38].position = {0.0f, size.y + float(m_border_size.y * 2)};
    m_vertices[39].position = {float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[40].position = {0.0f, size.y + float(m_border_size.y * 2)};
    m_vertices[41].position = {float(m_border_size.x), size.y + float(m_border_size.y * 2)};

    // bottom
    m_vertices[42].position = {float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[43].position = {size.x + float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[44].position = {float(m_border_size.x), size.y + float(m_border_size.y * 2)};
    m_vertices[45].position = {size.x + float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[46].position = {float(m_border_size.x), size.y + float(m_border_size.y * 2)};
    m_vertices[47].position
        = {size.x + float(m_border_size.x), size.y + float(m_border_size.y * 2)};

    // bottom right
    m_vertices[48].position = {size.x + float(m_border_size.x), size.y + float(m_border_size.y)};
    m_vertices[49].position
        = {size.x + float(m_border_size.x * 2), size.y + float(m_border_size.y)};
    m_vertices[50].position
        = {size.x + float(m_border_size.x), size.y + float(m_border_size.y * 2)};
    m_vertices[51].position
        = {size.x + float(m_border_size.x * 2), size.y + float(m_border_size.y)};
    m_vertices[52].position
        = {size.x + float(m_border_size.x), size.y + float(m_border_size.y * 2)};
    m_vertices[53].position
        = {size.x + float(m_border_size.x * 2), size.y + float(m_border_size.y * 2)};
}

void Frame::updateTexCoords()
{
    float left = static_cast<float>(m_frame_texture_rect.left);
    float right = left + m_frame_texture_rect.width;
    float top = static_cast<float>(m_frame_texture_rect.top);
    float bottom = top + m_frame_texture_rect.height;

    sf::Vector2f size;
    size.x = std::max(0.0f, float(m_frame_texture_rect.width - m_border_size.x * 2));
    size.y = std::max(0.0f, float(m_frame_texture_rect.height - m_border_size.y * 2));

    // left top
    m_vertices[0].texCoords = {left, top};
    m_vertices[1].texCoords = {left + m_border_size.x, top};
    m_vertices[2].texCoords = {left, top + m_border_size.y};
    m_vertices[3].texCoords = {left + m_border_size.x, top};
    m_vertices[4].texCoords = {left, top + m_border_size.y};
    m_vertices[5].texCoords = {left + m_border_size.x, top + m_border_size.y};

    // top
    m_vertices[6].texCoords = {left + m_border_size.x, top};
    m_vertices[7].texCoords = {size.x + left + m_border_size.x, top};
    m_vertices[8].texCoords = {left + m_border_size.x, top + m_border_size.y};
    m_vertices[9].texCoords = {size.x + left + m_border_size.x, top};
    m_vertices[10].texCoords = {left + m_border_size.x, top + m_border_size.y};
    m_vertices[11].texCoords = {size.x + left + m_border_size.x, top + m_border_size.y};

    // top right
    m_vertices[12].texCoords = {size.x + left + m_border_size.x, top};
    m_vertices[13].texCoords = {size.x + left + m_border_size.x * 2, top};
    m_vertices[14].texCoords = {size.x + left + m_border_size.x, top + m_border_size.y};
    m_vertices[15].texCoords = {size.x + left + m_border_size.x * 2, top};
    m_vertices[16].texCoords = {size.x + left + m_border_size.x, top + m_border_size.y};
    m_vertices[17].texCoords = {size.x + left + m_border_size.x * 2, top + m_border_size.y};

    // left
    m_vertices[18].texCoords = {left, top + m_border_size.y};
    m_vertices[19].texCoords = {left + m_border_size.x, top + m_border_size.y};
    m_vertices[20].texCoords = {left, size.y + top + m_border_size.y};
    m_vertices[21].texCoords = {left + m_border_size.x, top + m_border_size.y};
    m_vertices[22].texCoords = {left, size.y + top + m_border_size.y};
    m_vertices[23].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y};

    // center
    m_vertices[24].texCoords = {left + m_border_size.x, top + m_border_size.y};
    m_vertices[25].texCoords = {size.x + left + m_border_size.x, top + m_border_size.y};
    m_vertices[26].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[27].texCoords = {size.x + left + m_border_size.x, top + m_border_size.y};
    m_vertices[28].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[29].texCoords = {size.x + left + m_border_size.x, size.y + top + m_border_size.y};

    // right
    m_vertices[30].texCoords = {size.x + left + m_border_size.x, top + m_border_size.y};
    m_vertices[31].texCoords = {size.x + left + m_border_size.x * 2, top + m_border_size.y};
    m_vertices[32].texCoords = {size.x + left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[33].texCoords = {size.x + left + m_border_size.x * 2, top + m_border_size.y};
    m_vertices[34].texCoords = {size.x + left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[35].texCoords
        = {size.x + left + m_border_size.x * 2, size.y + top + m_border_size.y};

    // left bottom
    m_vertices[36].texCoords = {left, size.y + top + m_border_size.y};
    m_vertices[37].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[38].texCoords = {left, size.y + top + m_border_size.y * 2};
    m_vertices[39].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[40].texCoords = {left, size.y + top + m_border_size.y * 2};
    m_vertices[41].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y * 2};

    // bottom
    m_vertices[42].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[43].texCoords = {size.x + left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[44].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y * 2};
    m_vertices[45].texCoords = {size.x + left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[46].texCoords = {left + m_border_size.x, size.y + top + m_border_size.y * 2};
    m_vertices[47].texCoords
        = {size.x + left + m_border_size.x, size.y + top + m_border_size.y * 2};

    // bottom right
    m_vertices[48].texCoords = {size.x + left + m_border_size.x, size.y + top + m_border_size.y};
    m_vertices[49].texCoords
        = {size.x + left + m_border_size.x * 2, size.y + top + m_border_size.y};
    m_vertices[50].texCoords
        = {size.x + left + m_border_size.x, size.y + top + m_border_size.y * 2};
    m_vertices[51].texCoords
        = {size.x + left + m_border_size.x * 2, size.y + top + m_border_size.y};
    m_vertices[52].texCoords
        = {size.x + left + m_border_size.x, size.y + top + m_border_size.y * 2};
    m_vertices[53].texCoords
        = {size.x + left + m_border_size.x * 2, size.y + top + m_border_size.y * 2};
}

} // namespace fck::gui
