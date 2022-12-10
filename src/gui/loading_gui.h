#ifndef LOADINGGUI_KAZQSCZEMNSJ_H
#define LOADINGGUI_KAZQSCZEMNSJ_H

#include "gui_style.h"

#include "../gui_base.h"

namespace fck::gui
{

class LoadingGui : public GuiBase
{
public:
    LoadingGui();
    ~LoadingGui() = default;

    void setTotal(int32_t total);
    void increaseProgress();

    void resize(const sf::Vector2f &size);
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

private:
    int32_t m_total;
    int32_t m_progress;

    std::unique_ptr<ProgressBar> m_progress_bar;
};

} // namespace fck::gui

#endif // LOADINGGUI_KAZQSCZEMNSJ_H
