#include "loading_gui.h"

#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck::gui
{

LoadingGui::LoadingGui() : m_total{1}, m_progress{0}
{
    m_progress_bar.reset(Style::createLoadingProgressBar());
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

    m_progress_bar->setProgress(float(m_progress) / m_total);
    m_progress_bar->text().setString(
        std::to_string(int32_t(float(m_progress) / m_total) * 100) + "%");
}

void LoadingGui::resize(const sf::Vector2f &size)
{
    m_progress_bar->setPosition(
        {Style::viewport_offset.x,
         size.y - Style::viewport_offset.y - m_progress_bar->getGlobalBounds().getSize().y});
}

void LoadingGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(*m_progress_bar);
}

} // namespace fck::gui
