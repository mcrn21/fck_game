#include "splashscreen_gui.h"

#include "../fck/resource_cache.h"

namespace fck
{

SplashscreenGui::SplashscreenGui(sf::View *view) : m_view{view}
{
    m_spalshscreen_texture = ResourceCache::resource<sf::Texture>("splash");
}

void SplashscreenGui::draw()
{
    if (!m_view || !m_spalshscreen_texture)
        return;

    sf::FloatRect v{sf::Vector2f(0, 0), m_view->getSize()};

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(v.width, v.height));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin(
        "Splash screen", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    sf::Vector2f bg_size(v.width, v.height);
    sf::Vector2i bg_texture_size = sf::Vector2i(m_spalshscreen_texture->getSize());

    float window_ratio = float(v.width) / v.height;
    float texture_ratio = float(bg_texture_size.x) / bg_texture_size.y;

    if (window_ratio > texture_ratio)
    {
        bg_size.x = v.width;
        bg_size.y = bg_texture_size.y * (float(v.width) / bg_texture_size.x);
    }
    else
    {
        bg_size.y = v.height;
        bg_size.x = bg_texture_size.x * (float(v.height) / bg_texture_size.y);
    }

    ImGui::SetCursorPos(ImVec2((v.width - bg_size.x) / 2, (v.height - bg_size.y) / 2));
    ImGui::Image(*m_spalshscreen_texture, bg_size);

    ImGui::End();
    ImGui::PopStyleVar(2);
}

} // namespace fck
