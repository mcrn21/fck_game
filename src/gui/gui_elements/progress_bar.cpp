#include "progress_bar.h"

namespace fck::gui
{

ProgressBar::ProgressBar() : Frame{}, m_progress{1.0f}
{
}

ProgressBar::ProgressBar(
    sf::Texture &texture,
    const sf::IntRect &frame_texture_rect,
    const sf::IntRect &bar_texture_rect,
    const sf::Vector2f &bar_offset)
    : Frame{texture, frame_texture_rect},
      m_bar_texture_rect{bar_texture_rect},
      m_bar_offset{bar_offset},
      m_progress{1.0f}
{
    updateBarPositions();
    updateBarTexCoords();
}

void ProgressBar::setTexture(sf::Texture &texture)
{
    Frame::setTexture(texture);
    updateBarTexCoords();
}

const sf::IntRect &ProgressBar::getBarTextureRect() const
{
    return m_bar_texture_rect;
}

void ProgressBar::setBarTextureRect(const sf::IntRect &bar_texture_rect)
{
    m_bar_texture_rect = bar_texture_rect;
    updateBarTexCoords();
}

const sf::Vector2f &ProgressBar::getBarOffset() const
{
    return m_bar_offset;
}

void ProgressBar::setBarOffset(const sf::Vector2f &bar_offset)
{
    m_bar_offset = bar_offset;
    updateBarPositions();
}

void ProgressBar::setProgress(float progress)
{
    m_progress = progress;
    updateBarPositions();
}

sf::Text &ProgressBar::text()
{
    return m_text;
}

void ProgressBar::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    Frame::draw(target, states);

    if (getTexture())
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        new_state.transform *= getTransform();
        new_state.transform.translate(m_bar_offset);
        target.draw(m_bar_vertices, 4, sf::TriangleStrip, new_state);
        target.draw(m_text, new_state);
    }
}

void ProgressBar::updateBarPositions()
{
    sf::Vector2f size = getFrameSize() - m_bar_offset * 2.0f;

    size.x = std::max(0.0f, size.x) * m_progress;
    size.y = std::max(0.0f, size.y);

    m_bar_vertices[0].position = {0.0f, 0.0f};
    m_bar_vertices[1].position = {0.0f, size.y};
    m_bar_vertices[2].position = {size.x, 0.0f};
    m_bar_vertices[3].position = {size.x, size.y};
}

void ProgressBar::updateBarTexCoords()
{
    float left = static_cast<float>(m_bar_texture_rect.left);
    float right = left + m_bar_texture_rect.width * m_progress;
    float top = static_cast<float>(m_bar_texture_rect.top);
    float bottom = top + m_bar_texture_rect.height;

    m_bar_vertices[0].texCoords = sf::Vector2f(left, top);
    m_bar_vertices[1].texCoords = sf::Vector2f(left, bottom);
    m_bar_vertices[2].texCoords = sf::Vector2f(right, top);
    m_bar_vertices[3].texCoords = sf::Vector2f(right, bottom);
}

} // namespace fck::gui
