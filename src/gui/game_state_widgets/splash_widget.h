#ifndef SPLASHWIDGET_IYMEDFZCRGXQ_H
#define SPLASHWIDGET_IYMEDFZCRGXQ_H

#include "../widget.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class SplashWidget : public Widget
{
public:
    SplashWidget(Widget *parent = nullptr);
    ~SplashWidget() = default;
};

} // namespace fck::gui

#endif // SPLASHWIDGET_IYMEDFZCRGXQ_H
