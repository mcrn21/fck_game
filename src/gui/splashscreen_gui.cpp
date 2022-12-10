#include "splashscreen_gui.h"

#include "../fck/resource_cache.h"

namespace fck::gui
{

SplashscreenGui::SplashscreenGui()
{
    m_splashsceen_sprite.setTexture(*ResourceCache::resource<sf::Texture>("splash"), true);
}

void SplashscreenGui::resize(const sf::Vector2f &size)
{
    sf::Vector2f bg_size = size;
    sf::Vector2f bg_texture_size = m_splashsceen_sprite.getLocalBounds().getSize();

    float window_ratio = size.x / size.y;
    float texture_ratio = bg_texture_size.x / bg_texture_size.y;

    if (window_ratio > texture_ratio)
    {
        bg_size.x = size.x;
        bg_size.y = bg_texture_size.y * (size.x / bg_texture_size.x);
    }
    else
    {
        bg_size.y = size.y;
        bg_size.x = bg_texture_size.x * (size.y / bg_texture_size.y);
    }

    m_splashsceen_sprite.setPosition({(size.x - bg_size.x) / 2, (size.y - bg_size.y) / 2});
}

void SplashscreenGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_splashsceen_sprite);
}

} // namespace fck::gui
