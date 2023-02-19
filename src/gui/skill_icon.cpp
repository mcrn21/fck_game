#include "skill_icon.h"

#include "../knowledge_base/knowledge_base.h"

#include "../fck/resource_cache.h"

namespace fck::gui
{

SkillIcon::SkillIcon(const SkillBase &skill, Widget *parent) : Widget{parent}, m_skill{&skill}
{
    setWidgetTheme(WidgetTheme::get<SkillIcon>());

    KnowledgeBase::SkillItemBase *skill_item = KnowledgeBase::getSkill(m_skill->getName());

    m_skill_icon.setTexture(*ResourceCache::get<sf::Texture>(skill_item->getTextureName()));
    m_skill_icon.setTextureRect(skill_item->getTextureRect());

    setSize({96.0f, 96.0f});
}

const SkillBase *SkillIcon::getSkill() const
{
    return m_skill;
}

void SkillIcon::setActivated(bool activated)
{
    m_skill_icon.setColor(activated ? sf::Color(255, 255, 255, 127) : sf::Color::White);
}

void SkillIcon::onResized(const sf::Vector2f &size)
{
    m_background.setSize(size);
    updateGeometry();
}

void SkillIcon::onThemeChanged(const WidgetTheme &widget_theme)
{
    m_background.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_background.setBorderSize(widget_theme.background.border_size);
    m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
    m_background.setBorderTextureSize(widget_theme.background.texture_border_size);

    updateGeometry();
}

void SkillIcon::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= getTransform();
    target.draw(m_background, new_states);
    target.draw(m_skill_icon, new_states);
}

void SkillIcon::updateGeometry()
{
    const WidgetTheme &widget_theme = getWidgetTheme();

    m_skill_icon.setSize(
        getSize() - widget_theme.padding.getLowerPoint() - widget_theme.padding.getUpperPoint());
    m_skill_icon.setPoisition(widget_theme.padding.getLowerPoint());
}

} // namespace fck::gui
