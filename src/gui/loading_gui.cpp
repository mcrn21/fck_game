#include "loading_gui.h"

#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck::gui
{

LoadingGui::LoadingGui(const sf::Vector2f &size) : m_size{size}, m_total{1}, m_progress{0}
{
    m_scale = {4.0f, 4.0f};
    m_border_offset = {20.0f, 40.0f};

    //    m_progress_bar.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    //    m_progress_bar.setBackgroundTextureRect(sf::IntRect{{0, 0}, {76, 8}});
    //    m_progress_bar.setBarTextureRect(sf::IntRect{{2, 29}, {68, 3}});
    //    m_progress_bar.setBarPosition({2.0f, 2.0f});
    //    m_progress_bar.setOrigin({0.0f, m_progress_bar.localBounds().height});
    //    m_progress_bar.setScale(m_scale);

    //    m_progress_bar.text().setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    //    m_progress_bar.text().setCharacterSize(30);
    //    m_progress_bar.text().setOutlineColor(sf::Color::Black);
    //    m_progress_bar.text().setOutlineThickness(1.0f);
    //    m_progress_bar.text().setScale(vector2::div({1.0f, 1.0f}, m_scale));
    //    m_progress_bar.text().setPosition({m_progress_bar.localBounds().width, -2.0f});

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

    m_progress_bar.setProgress(float(m_progress) / m_total);
    m_progress_bar.text().setString(
        std::to_string(int32_t(float(m_progress) / m_total) * 100) + "%");
}

void LoadingGui::resize(const sf::Vector2f &size)
{
    m_size = size;

    m_progress_bar.setPosition({m_border_offset.x, m_size.y - m_border_offset.y});
}

void LoadingGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    //    target.draw(m_progress_bar);
}

} // namespace fck::gui
