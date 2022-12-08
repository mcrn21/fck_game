#include "progress_bar.h"

namespace fck
{

ProgressBar::ProgressBar() : m_texture{nullptr}, m_progress{1.0f}
{
}

ProgressBar::ProgressBar(
    sf::Texture &texture,
    const sf::IntRect &background_texture_rect,
    const sf::IntRect &bar_texture_rect,
    const sf::Vector2f &bar_position)
    : m_texture{&texture},
      m_background_texture_rect{background_texture_rect},
      m_bar_texture_rect{bar_texture_rect},
      m_bar_position{bar_position},
      m_progress{1.0f}
{
    updateBackgroundPositions();
    updateBackgroundTexCoords();

    updateBarPositions();
    updateBarTexCoords();
}

void ProgressBar::setTexture(sf::Texture &texture)
{
    m_texture = &texture;
}

void ProgressBar::setBackgroundTextureRect(const sf::IntRect &background_texture_rect)
{
    m_background_texture_rect = background_texture_rect;
    updateBackgroundTexCoords();
}

void ProgressBar::setBarTextureRect(const sf::IntRect &bar_texture_rect)
{
    m_bar_texture_rect = bar_texture_rect;
    updateBarTexCoords();
}

void ProgressBar::setBarPosition(const sf::Vector2f &bar_position)
{
    m_bar_position = bar_position;
    updateBackgroundPositions();
}

void ProgressBar::setProgress(float progress)
{
    m_progress = progress;
    updateBarPositions();
    updateBarTexCoords();
}

sf::Text &ProgressBar::text()
{
    return m_text;
}

sf::FloatRect ProgressBar::localBounds() const
{
    auto width = static_cast<float>(std::abs(m_background_texture_rect.width));
    auto height = static_cast<float>(std::abs(m_background_texture_rect.height));

    return sf::FloatRect({0.f, 0.f}, {width, height});
}

sf::FloatRect ProgressBar::globalBounds() const
{
    return getTransform().transformRect(localBounds());
}

void ProgressBar::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    if (m_texture)
    {
        sf::RenderStates new_state = states;
        new_state.texture = m_texture;
        new_state.transform *= getTransform();

        target.draw(m_background_vertices, 4, sf::TriangleStrip, new_state);
        target.draw(m_bar_vertices, 4, sf::TriangleStrip, new_state);

        //        sf::Vector2f scale = {1.0f / getScale().x, 1.0f / getScale().y};
        //        new_state.transform.scale(scale);
        target.draw(m_text, new_state);
    }
}

void ProgressBar::updateBackgroundPositions()
{
    m_background_vertices[0].position = sf::Vector2f(0, 0);
    m_background_vertices[1].position = sf::Vector2f(0, m_background_texture_rect.height);
    m_background_vertices[2].position = sf::Vector2f(m_background_texture_rect.width, 0);
    m_background_vertices[3].position
        = sf::Vector2f(m_background_texture_rect.width, m_background_texture_rect.height);
}

void ProgressBar::updateBackgroundTexCoords()
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

void ProgressBar::updateBarPositions()
{
    m_bar_vertices[0].position = m_bar_position;
    m_bar_vertices[1].position = sf::Vector2f(0, m_bar_texture_rect.height) + m_bar_position;
    m_bar_vertices[2].position
        = sf::Vector2f(m_bar_texture_rect.width * m_progress, 0) + m_bar_position;
    m_bar_vertices[3].position
        = sf::Vector2f(m_bar_texture_rect.width * m_progress, m_bar_texture_rect.height)
        + m_bar_position;
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

} // namespace fck
