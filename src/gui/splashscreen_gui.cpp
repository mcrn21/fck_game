#include "splashscreen_gui.h"

#include "../fck/resource_cache.h"

namespace fck::gui
{

SplashscreenGui::SplashscreenGui(const sf::Vector2f &size) : m_size{size}
{
    m_splashsceen_sprite.setTexture(*ResourceCache::resource<sf::Texture>("splash"), true);
    resize(m_size);
}

void SplashscreenGui::resize(const sf::Vector2f &size)
{
    m_size = size;

    sf::Vector2f bg_size = m_size;
    sf::Vector2f bg_texture_size = m_splashsceen_sprite.getLocalBounds().getSize();

    float window_ratio = m_size.x / m_size.y;
    float texture_ratio = bg_texture_size.x / bg_texture_size.y;

    if (window_ratio > texture_ratio)
    {
        bg_size.x = m_size.x;
        bg_size.y = bg_texture_size.y * (m_size.x / bg_texture_size.x);
    }
    else
    {
        bg_size.y = m_size.y;
        bg_size.x = bg_texture_size.x * (m_size.y / bg_texture_size.y);
    }

    m_splashsceen_sprite.setPosition({(m_size.x - bg_size.x) / 2, (m_size.y - bg_size.y) / 2});
}

void SplashscreenGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_splashsceen_sprite);
}

} // namespace fck::gui
