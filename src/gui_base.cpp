#include "gui_base.h"

namespace fck
{

GuiBase::GuiBase() : m_hovered_frame{nullptr}, m_focused_frame{nullptr}, m_destroy{false}
{
}

GuiBase::~GuiBase()
{
    m_destroy = true;
}

void GuiBase::addFrame(gui::Frame *frame)
{
    if (!frame)
        return;

    int32_t proxy_id = m_frames_tree.createProxy(frame->getGlobalBounds(), frame);
    frame->m_proxy_id = proxy_id;
    frame->m_gui_base = this;
    frame->m_z_order = m_frames.size();

    m_frames.push_back({proxy_id, std::unique_ptr<gui::Frame>(frame)});
    m_frames.sort(
        [](const auto &a, const auto &b) { return a.second->m_z_order < b.second->m_z_order; });

    for (gui::Frame *child : frame->m_children)
        addFrame(child);
}

void GuiBase::removeFrame(gui::Frame *frame)
{
    if (!frame)
        return;

    auto frames_found = std::find_if(m_frames.begin(), m_frames.end(), [frame](const auto &v) {
        return v.second.get() == frame;
    });

    if (frames_found == m_frames.end())
        return;

    m_frames_tree.destroyProxy(frames_found->first);
    frames_found->second.release();
    frame->m_proxy_id = -1;
    frame->m_gui_base = nullptr;

    if (m_hovered_frame == frame)
        m_hovered_frame = nullptr;

    if (m_focused_frame == frame)
        m_focused_frame = nullptr;

    m_frames.erase(frames_found);

    for (gui::Frame *child : frame->m_children)
        removeFrame(child);
}

void GuiBase::moveFrame(gui::Frame *frame, const sf::Vector2f &offset)
{
    if (!frame || frame->m_proxy_id == -1 || frame->m_gui_base != this)
        return;

    m_frames_tree.moveProxy(frame->m_proxy_id, frame->getGlobalBounds(), offset);
}

void GuiBase::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    for (const auto &frame : m_frames)
    {
        if (frame.second->isVisible())
        {
            //            sf::FloatRect rect;
            //            auto aabb = m_frames_tree.fatAABB(frame.first);
            //            rect.left = aabb.lower_bound.x;
            //            rect.top = aabb.lower_bound.y;
            //            rect.width = aabb.upper_bound.x - aabb.lower_bound.x;
            //            rect.height = aabb.upper_bound.y - aabb.lower_bound.y;

            //            sf::RectangleShape rs(rect.getSize());
            //            rs.setFillColor(sf::Color::Transparent);
            //            rs.setOutlineColor(sf::Color::White);
            //            rs.setOutlineThickness(1);
            //            rs.setPosition(rect.getPosition());
            //            target.draw(rs, states);

            target.draw(*frame.second, states);
        }
    }
}

void GuiBase::event(Event *event)
{
    const SfmlEvent *sf_event = static_cast<SfmlEvent *>(event);

    if (sf_event->get().type == sf::Event::MouseMoved)
    {
        gui::Frame *hovered_frame = nullptr;

        m_hovered_frames.clear();
        sf::FloatRect bounds = {
            {float(sf_event->get().mouseMove.x), float(sf_event->get().mouseMove.y)}, {1.0f, 1.0f}};
        m_frames_tree.querry(bounds, [&](int32_t proxy_id) {
            gui::Frame *f = m_frames_tree.getUserData(proxy_id);
            if (f->isVisible())
                m_hovered_frames.push_back(f);

            return true;
        });

        if (!m_hovered_frames.empty())
        {
            m_hovered_frames.sort([](const gui::Frame *a, const gui::Frame *b) {
                return a->m_z_order < b->m_z_order;
            });
            hovered_frame = m_hovered_frames.back();
        }

        if (hovered_frame)
        {
            if (hovered_frame != m_hovered_frame)
            {
                if (m_hovered_frame)
                {
                    m_hovered_frame->m_hovered = false;
                    m_hovered_frame->onHoverOut();
                }

                if (hovered_frame->isEnable())
                {
                    m_hovered_frame = hovered_frame;
                    m_hovered_frame->m_hovered = true;
                    m_hovered_frame->onHoverIn();
                }
            }
        }
        else
        {
            if (m_hovered_frame)
            {
                m_hovered_frame->m_hovered = false;
                m_hovered_frame->onHoverOut();
                m_hovered_frame = nullptr;
            }
        }

        return;
    }

    if (sf_event->get().type == sf::Event::MouseButtonPressed)
    {
        if (m_hovered_frame)
        {
            if (m_focused_frame && m_focused_frame != m_hovered_frame)
            {
                m_focused_frame->m_focused = false;
                m_focused_frame->onFocusOut();
                m_focused_frame = nullptr;
            }

            if (m_hovered_frame->isFocusable())
            {
                m_focused_frame = m_hovered_frame;
                m_focused_frame->m_focused = true;
                m_focused_frame->onFocusIn();
            }

            if (m_hovered_frame->isEnable())
                m_hovered_frame->onMousePressed();
        }
        else
        {
            if (m_focused_frame)
            {
                m_focused_frame->m_focused = false;
                m_focused_frame->onFocusOut();
                m_focused_frame = nullptr;
            }
        }

        return;
    }

    if (sf_event->get().type == sf::Event::MouseButtonReleased)
    {
        if (m_hovered_frame)
            m_hovered_frame->onMouseReleased();
        return;
    }
}

} // namespace fck
