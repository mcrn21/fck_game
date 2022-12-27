#ifndef LOADINGWIDGET_OSYVVHLAGHED_H
#define LOADINGWIDGET_OSYVVHLAGHED_H

#include "../progress_bar.h"
#include "../widget.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class LoadingWidget : public Widget
{
public:
    LoadingWidget(Widget *parent = nullptr);
    ~LoadingWidget() = default;

    int32_t getTotal() const;
    void setTotal(int32_t total);

    void next();

protected:
    void onWindowResized(const sf::Vector2f &size) override;

private:
    void updateGeometry();

protected:
    int32_t m_progress;
    int32_t m_total;
    ProgressBar *m_progress_bar;
};

} // namespace fck::gui

#endif // LOADINGWIDGET_OSYVVHLAGHED_H
