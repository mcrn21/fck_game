#include "clipping.h"

#include <cmath>

namespace fck
{

sf::View Clipping::m_original_view;

Clipping::Clipping(
    sf::RenderTarget &target,
    const sf::RenderStates &states,
    sf::Vector2f top_left,
    sf::Vector2f size)
    : m_target(target), m_old_view(target.getView())
{
    sf::Vector2f bottom_right = sf::Vector2f(states.transform.transformPoint(top_left + size));
    top_left = sf::Vector2f(states.transform.transformPoint(top_left));

    sf::Vector2f view_top_left = top_left;
    size = bottom_right - top_left;

    top_left.x -= m_original_view.getCenter().x - (m_original_view.getSize().x / 2.f);
    top_left.y -= m_original_view.getCenter().y - (m_original_view.getSize().y / 2.f);
    bottom_right.x -= m_original_view.getCenter().x - (m_original_view.getSize().x / 2.f);
    bottom_right.y -= m_original_view.getCenter().y - (m_original_view.getSize().y / 2.f);

    top_left.x *= m_original_view.getViewport().width / m_original_view.getSize().x;
    top_left.y *= m_original_view.getViewport().height / m_original_view.getSize().y;
    size.x *= m_original_view.getViewport().width / m_original_view.getSize().x;
    size.y *= m_original_view.getViewport().height / m_original_view.getSize().y;

    top_left.x += m_original_view.getViewport().left;
    top_left.y += m_original_view.getViewport().top;

    if (top_left.x < m_old_view.getViewport().left)
    {
        size.x -= m_old_view.getViewport().left - top_left.x;
        view_top_left.x += (m_old_view.getViewport().left - top_left.x)
            * (m_original_view.getSize().x / m_original_view.getViewport().width);
        top_left.x = m_old_view.getViewport().left;
    }

    if (top_left.y < m_old_view.getViewport().top)
    {
        size.y -= m_old_view.getViewport().top - top_left.y;
        view_top_left.y += (m_old_view.getViewport().top - top_left.y)
            * (m_original_view.getSize().y / m_original_view.getViewport().height);
        top_left.y = m_old_view.getViewport().top;
    }

    if (size.x > m_old_view.getViewport().left + m_old_view.getViewport().width - top_left.x)
        size.x = m_old_view.getViewport().left + m_old_view.getViewport().width - top_left.x;
    if (size.y > m_old_view.getViewport().top + m_old_view.getViewport().height - top_left.y)
        size.y = m_old_view.getViewport().top + m_old_view.getViewport().height - top_left.y;

    if ((size.x >= 0) && (size.y >= 0))
    {
        sf::View view{
            {{std::round(view_top_left.x), std::round(view_top_left.y)},
             {std::round(
                  size.x * m_original_view.getSize().x / m_original_view.getViewport().width),
              std::round(
                  size.y * m_original_view.getSize().y / m_original_view.getViewport().height)}}};

        view.setViewport({{top_left.x, top_left.y}, {size.x, size.y}});
        target.setView(view);
    }
    else
    {
        sf::View emptyView{{{0, 0}, {0, 0}}};
        emptyView.setViewport({{0, 0}, {0, 0}});
        target.setView(emptyView);
    }
}

Clipping::~Clipping()
{
    m_target.setView(m_old_view);
}

void Clipping::setOriginalView(const sf::View &view)
{
    m_original_view = view;
}

} // namespace fck
