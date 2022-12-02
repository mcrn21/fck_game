#include "drawable.h"

namespace fck
{

Drawable::Drawable() : m_visible{true}
{
}

drawable_type::Type Drawable::type() const
{
    return drawable_type::NO_TYPE;
}

bool Drawable::isVisible() const
{
    return m_visible;
}

void Drawable::setVisible(bool visible)
{
    m_visible = visible;
}

} // namespace fck
