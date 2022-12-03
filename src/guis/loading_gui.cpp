#include "loading_gui.h"

#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck
{

LoadingGui::LoadingGui(const sf::Vector2f &size) : m_size{size}, m_total{1}, m_progress{0}
{
    m_scale = {4.0f, 4.0f};
    m_border_offset = {20.0f, 40.0f};

    m_progress_width = 68.0f;

    m_progress_bg_sprite.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_progress_bg_sprite.setTextureRect(sf::IntRect{{0, 0}, {76, 8}});
    m_progress_bg_sprite.setOrigin({0.0f, m_progress_bg_sprite.getLocalBounds().height});
    m_progress_bg_sprite.scale(m_scale);

    m_progress_sprite.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_progress_sprite.setTextureRect(sf::IntRect{{2, 29}, {68, 3}});
    m_progress_sprite.setOrigin({0.0f, m_progress_sprite.getLocalBounds().height});
    m_progress_sprite.scale(m_scale);

    m_progress_text.setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_progress_text.setString(" ");
    m_progress_text.setCharacterSize(30);
    m_progress_text.setOutlineColor(sf::Color::Black);
    m_progress_text.setOutlineThickness(1.0f);
    m_progress_text.setOrigin({0.0f, m_progress_text.getLocalBounds().height});

    resize(m_size);
}

void LoadingGui::setTotal(int32_t total)
{
    m_total = total;

    if (m_total < 1)
        m_total = 1;
}

void LoadingGui::increaseProgress()
{
    ++m_progress;
    if (m_progress > m_total)
        m_progress = m_total;

    sf::IntRect progress_rect = m_progress_sprite.getTextureRect();
    progress_rect.width = m_progress_width * (float(m_progress) / m_total);
    m_progress_sprite.setTextureRect(progress_rect);

    m_progress_text.setString(std::to_string(int32_t(float(m_progress) / m_total) * 100) + "%");
}

void LoadingGui::resize(const sf::Vector2f &size)
{
    m_size = size;

    m_progress_bg_sprite.setPosition({m_border_offset.x, m_size.y - m_border_offset.y});
    m_progress_sprite.setPosition(
        sf::Vector2f{m_border_offset.x, m_size.y - m_border_offset.y}
        + vector2::mult({2.0f, -3.0f}, m_scale));

    m_progress_text.setPosition(
        {m_border_offset.x + m_progress_bg_sprite.getGlobalBounds().width + 1.0f,
         m_size.y - m_border_offset.y * 1.95f});
}

void LoadingGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_progress_bg_sprite);
    target.draw(m_progress_sprite);
    target.draw(m_progress_text);
}

} // namespace fck
