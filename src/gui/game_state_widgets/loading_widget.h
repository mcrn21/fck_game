#ifndef LOADINGWIDGET_OSYVVHLAGHED_H
#define LOADINGWIDGET_OSYVVHLAGHED_H

#include "../widget.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class LoadingWidget : public Widget
{
public:
    LoadingWidget(Widget *parent = nullptr);
    ~LoadingWidget() = default;
};

} // namespace fck::gui

#endif // LOADINGWIDGET_OSYVVHLAGHED_H
