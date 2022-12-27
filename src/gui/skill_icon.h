#ifndef SKILLICON_WQRPSLIJOOXW_H
#define SKILLICON_WQRPSLIJOOXW_H

#include "box.h"
#include "widget.h"

#include "../skills/skill_base.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class SkillIcon : public Widget
{
public:
    SkillIcon(const SkillBase &skill, Widget *parent = nullptr);
    ~SkillIcon() = default;

    const SkillBase *getSkill() const;

    void setActivated(bool activated);

protected:
    void onResized(const sf::Vector2f &size) override;
    void onThemeChanged(const WidgetTheme &widget_theme) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateGeometry();

private:
    const SkillBase *m_skill;
    Box m_background;
    Box m_skill_icon;
};

} // namespace fck::gui

#endif // SKILLICON_WQRPSLIJOOXW_H
