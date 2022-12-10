#include "frame.h"

#include "../../gui_base.h"

namespace fck::gui
{

Frame::Frame()
    : m_texture{nullptr},
      m_visible{true},
      m_enable{true},
      m_hovered{false},
      m_focused{false},
      m_focusable{false},
      m_parent{nullptr},
      m_proxy_id{-1},
      m_gui_base{nullptr},
      m_z_order{-1}
{
}

Frame::Frame(sf::Texture &texture, const sf::IntRect &frame_texture_rect)
    : m_texture{&texture},
      m_frame_texture_rect{frame_texture_rect},
      m_visible{true},
      m_enable{true},
      m_hovered{false},
      m_focused{false},
      m_focusable{false},
      m_parent{nullptr},
      m_proxy_id{-1},
      m_gui_base{nullptr},
      m_z_order{-1}
{
    updatePositions();
    updateTexCoords();
}

Frame::~Frame()
{
    spdlog::debug("Remove frame");
    if (m_gui_base && !m_gui_base->m_destroy)
    {
        m_gui_base->removeFrame(this);

        for (Frame *child : m_children)
            delete child;
    }
}

void Frame::setPosition(const sf::Vector2f &position)
{
    sf::Transformable::setPosition(position);
    updateFramesTree(sf::Vector2f{});
    updateParentsTransform();
}

void Frame::setRotation(sf::Angle angle)
{
    sf::Transformable::setRotation(angle);
    updateFramesTree(sf::Vector2f{});
    updateParentsTransform();
}

void Frame::setScale(const sf::Vector2f &factors)
{
    sf::Transformable::setScale(factors);
    updateFramesTree(sf::Vector2f{});
    updateParentsTransform();
}

void Frame::setOrigin(const sf::Vector2f &origin)
{
    sf::Transformable::setOrigin(origin);
    updateFramesTree(sf::Vector2f{});
    updateParentsTransform();
}

void Frame::move(const sf::Vector2f &offset)
{
    sf::Transformable::move(offset);
    updateFramesTree(offset);
}

void Frame::rotate(sf::Angle angle)
{
    sf::Transformable::rotate(angle);
    updateFramesTree(sf::Vector2f{});
    updateParentsTransform();
}

void Frame::scale(const sf::Vector2f &factor)
{
    sf::Transformable::scale(factor);
    updateFramesTree(sf::Vector2f{});
    updateParentsTransform();
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

const sf::Color &Frame::getFrameColor() const
{
    return m_vertices[0].color;
}

void Frame::setFrameColor(const sf::Color &color)
{
    for (int32_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = color;
}

sf::FloatRect Frame::getLocalBounds() const
{
    return m_vertices.getBounds();
}

sf::FloatRect Frame::getGlobalBounds() const
{
    sf::Transform transform;

    if (m_parent)
    {
        for (int32_t i = m_parents.size() - 1; i >= 0; --i)
            transform *= m_parents[i]->getTransform();
    }
    transform *= getTransform();

    return transform.transformRect(getLocalBounds());
}

bool Frame::isVisible() const
{
    return m_visible;
}

void Frame::setVisible(bool visible)
{
    m_visible = visible;
}

bool Frame::isEnable() const
{
    return m_enable;
}

void Frame::setEnable(bool enable)
{
    m_enable = enable;
}

bool Frame::isHovered() const
{
    return m_hovered;
}

bool Frame::isFocused() const
{
    return m_focused;
}

bool Frame::isFocusable() const
{
    return m_focusable;
}

void Frame::setFocusable(bool focusabel)
{
    m_focusable = focusabel;
}

void Frame::setParent(Frame *frame)
{
    if (m_parent)
    {
        m_parent->m_children.erase(
            std::remove(m_parent->m_children.begin(), m_parent->m_children.end(), this),
            m_parent->m_children.end());
    }

    m_parents.clear();
    m_parent = frame;

    if (m_parent)
    {
        m_parent->m_children.push_back(this);
        Frame *p = m_parent;
        while (p)
        {
            m_parents.push_back(p);
            p = p->m_parent;
        }
    }
}

const sf::Transform &Frame::getParentsTransform() const
{
    return m_parents_transform;
}

void Frame::onHoverIn()
{
}

void Frame::onHoverOut()
{
}

void Frame::onFocusIn()
{
}

void Frame::onFocusOut()
{
}

void Frame::onMousePressed()
{
}

void Frame::onMouseReleased()
{
}

void Frame::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;

        if (m_parent)
        {
            for (int32_t i = m_parents.size() - 1; i >= 0; --i)
                new_state.transform *= m_parents[i]->getTransform();
        }

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
    if (m_vertices.getVertexCount() == 0)
        return;

    float left = static_cast<float>(m_frame_texture_rect.left);
    float right = left + m_frame_texture_rect.width;
    float top = static_cast<float>(m_frame_texture_rect.top);
    float bottom = top + m_frame_texture_rect.height;

    sf::Vector2f border_size;
    border_size.x = std::min(float(m_frame_texture_rect.width), float(m_border_size.x));
    border_size.y = std::min(float(m_frame_texture_rect.height), float(m_border_size.y));

    sf::Vector2f size;
    size.x = std::max(0.0f, float(m_frame_texture_rect.width - border_size.x * 2));
    size.y = std::max(0.0f, float(m_frame_texture_rect.height - border_size.y * 2));

    // left top
    m_vertices[0].texCoords = {left, top};
    m_vertices[1].texCoords = {left + border_size.x, top};
    m_vertices[2].texCoords = {left, top + border_size.y};
    m_vertices[3].texCoords = {left + border_size.x, top};
    m_vertices[4].texCoords = {left, top + border_size.y};
    m_vertices[5].texCoords = {left + border_size.x, top + border_size.y};

    // top
    m_vertices[6].texCoords = {left + border_size.x, top};
    m_vertices[7].texCoords = {size.x + left + border_size.x, top};
    m_vertices[8].texCoords = {left + border_size.x, top + border_size.y};
    m_vertices[9].texCoords = {size.x + left + border_size.x, top};
    m_vertices[10].texCoords = {left + border_size.x, top + border_size.y};
    m_vertices[11].texCoords = {size.x + left + border_size.x, top + border_size.y};

    // top right
    m_vertices[12].texCoords = {size.x + left + border_size.x, top};
    m_vertices[13].texCoords = {size.x + left + border_size.x * 2, top};
    m_vertices[14].texCoords = {size.x + left + border_size.x, top + border_size.y};
    m_vertices[15].texCoords = {size.x + left + border_size.x * 2, top};
    m_vertices[16].texCoords = {size.x + left + border_size.x, top + border_size.y};
    m_vertices[17].texCoords = {size.x + left + border_size.x * 2, top + border_size.y};

    // left
    m_vertices[18].texCoords = {left, top + border_size.y};
    m_vertices[19].texCoords = {left + border_size.x, top + border_size.y};
    m_vertices[20].texCoords = {left, size.y + top + border_size.y};
    m_vertices[21].texCoords = {left + border_size.x, top + border_size.y};
    m_vertices[22].texCoords = {left, size.y + top + border_size.y};
    m_vertices[23].texCoords = {left + border_size.x, size.y + top + border_size.y};

    // center
    m_vertices[24].texCoords = {left + border_size.x, top + border_size.y};
    m_vertices[25].texCoords = {size.x + left + border_size.x, top + border_size.y};
    m_vertices[26].texCoords = {left + border_size.x, size.y + top + border_size.y};
    m_vertices[27].texCoords = {size.x + left + border_size.x, top + border_size.y};
    m_vertices[28].texCoords = {left + border_size.x, size.y + top + border_size.y};
    m_vertices[29].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y};

    // right
    m_vertices[30].texCoords = {size.x + left + border_size.x, top + border_size.y};
    m_vertices[31].texCoords = {size.x + left + border_size.x * 2, top + border_size.y};
    m_vertices[32].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y};
    m_vertices[33].texCoords = {size.x + left + border_size.x * 2, top + border_size.y};
    m_vertices[34].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y};
    m_vertices[35].texCoords = {size.x + left + border_size.x * 2, size.y + top + border_size.y};

    // left bottom
    m_vertices[36].texCoords = {left, size.y + top + border_size.y};
    m_vertices[37].texCoords = {left + border_size.x, size.y + top + border_size.y};
    m_vertices[38].texCoords = {left, size.y + top + border_size.y * 2};
    m_vertices[39].texCoords = {left + border_size.x, size.y + top + border_size.y};
    m_vertices[40].texCoords = {left, size.y + top + border_size.y * 2};
    m_vertices[41].texCoords = {left + border_size.x, size.y + top + border_size.y * 2};

    // bottom
    m_vertices[42].texCoords = {left + border_size.x, size.y + top + border_size.y};
    m_vertices[43].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y};
    m_vertices[44].texCoords = {left + border_size.x, size.y + top + border_size.y * 2};
    m_vertices[45].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y};
    m_vertices[46].texCoords = {left + border_size.x, size.y + top + border_size.y * 2};
    m_vertices[47].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y * 2};

    // bottom right
    m_vertices[48].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y};
    m_vertices[49].texCoords = {size.x + left + border_size.x * 2, size.y + top + border_size.y};
    m_vertices[50].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y * 2};
    m_vertices[51].texCoords = {size.x + left + border_size.x * 2, size.y + top + border_size.y};
    m_vertices[52].texCoords = {size.x + left + border_size.x, size.y + top + border_size.y * 2};
    m_vertices[53].texCoords
        = {size.x + left + border_size.x * 2, size.y + top + border_size.y * 2};
}

void Frame::updateFramesTree(const sf::Vector2f &offset)
{
    if (m_gui_base)
    {
        m_gui_base->moveFrame(this, offset);
        for (Frame *child : m_children)
            m_gui_base->moveFrame(child, offset);
    }
}

void Frame::updateParentsTransform()
{
    sf::Transform transform;
    if (m_parent)
    {
        for (int32_t i = m_parents.size() - 1; i >= 0; --i)
            transform *= m_parents[i]->getTransform();
    }
    transform *= getTransform();
    m_parents_transform = transform;

    for (Frame *child : m_children)
        child->updateParentsTransform();
}

} // namespace fck::gui
