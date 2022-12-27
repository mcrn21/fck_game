#include "loading_widget.h"

#include <spdlog/spdlog.h>

namespace fck::gui
{

LoadingWidget::LoadingWidget(Widget *parent) : Widget{parent}, m_progress{1}, m_total{1}
{
    WidgetTheme widget_theme = getWidgetTheme();
    widget_theme.padding = 0.0f;
    setWidgetTheme(widget_theme);

    setSize(Widget::getWindowSize());
    setSelectable(false);

    m_progress_bar = new ProgressBar{this};
    m_progress_bar->setSize({256.0f, 28.0f});

    WidgetTheme progress_bar_theme = m_progress_bar->getWidgetTheme();

    progress_bar_theme.foreground.texture_rects[WidgetState::DEFAULT] = {{96, 96}, {32, 32}};
    progress_bar_theme.foreground.texture_rects[WidgetState::HOVERED] = {{96, 96}, {32, 32}};
    progress_bar_theme.foreground.texture_rects[WidgetState::PRESSED] = {{96, 96}, {32, 32}};
    progress_bar_theme.foreground.texture_rects[WidgetState::FOCUSED] = {{96, 96}, {32, 32}};

    progress_bar_theme.text.align = TextAlign::LEFT | TextAlign::V_CENTER;

    m_progress_bar->setTextOffset({10.0f, 0.0f});
    m_progress_bar->setWidgetTheme(progress_bar_theme);

    m_progress_bar->setProgress(0.0f);

    updateGeometry();
}

int32_t LoadingWidget::getTotal() const
{
    return m_total;
}

void LoadingWidget::setTotal(int32_t total)
{
    m_total = total;
    m_progress = 0;
    next();
}

void LoadingWidget::next()
{
    if (m_progress != m_total)
    {
        ++m_progress;
        m_progress_bar->setProgress(float(m_progress) / m_total);
    }
}

void LoadingWidget::onWindowResized(const sf::Vector2f &size)
{
    setSize(size);
    updateGeometry();
}

void LoadingWidget::updateGeometry()
{
    Sides<float> viewport_padding = 30.0f;

    m_progress_bar->setPosition(
        {viewport_padding.left,
         getSize().y - viewport_padding.bottom - m_progress_bar->getSize().y});
}

} // namespace fck::gui
