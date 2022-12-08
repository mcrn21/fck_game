#ifndef LOADINGGUI_H
#define LOADINGGUI_H

#include "gui_elements/gui_elements.h"

#include "../gui_base.h"

namespace fck
{

class LoadingGui : public GuiBase
{
public:
    LoadingGui(const sf::Vector2f &size);
    ~LoadingGui() = default;

    void setTotal(int32_t total);
    void increaseProgress();

    void resize(const sf::Vector2f &size);
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

private:
    sf::Vector2f m_size;

    int32_t m_total;
    int32_t m_progress;

    sf::Vector2f m_scale;
    sf::Vector2f m_border_offset;

    ProgressBar m_progress_bar;
};

} // namespace fck

#endif // LOADINGGUI_H
