#ifndef GUIBASE_RMDPTERCJQWN_H
#define GUIBASE_RMDPTERCJQWN_H

#include "fck_common.h"

#include "gui/gui_elements/frame.h"

#include "fck/b2_dynamic_tree.h"

#include <SFML/Graphics.hpp>

#include <list>
#include <vector>

namespace fck
{

class GuiBase
{
    friend class gui::Frame;

public:
    GuiBase();
    virtual ~GuiBase();

    void addFrame(gui::Frame *frame);
    void removeFrame(gui::Frame *frame);
    void moveFrame(gui::Frame *frame, const sf::Vector2f &offset);

    virtual void resize(const sf::Vector2f &size) = 0;
    virtual void draw(sf::RenderTarget &target, const sf::RenderStates &states);

    void event(Event *event);

private:
    std::list<std::pair<int32_t, std::unique_ptr<gui::Frame>>> m_frames;
    b2::DynamicTree<gui::Frame *> m_frames_tree;

    std::list<gui::Frame *> m_hovered_frames;
    gui::Frame *m_hovered_frame;
    gui::Frame *m_focused_frame;

    bool m_destroy;
};

} // namespace fck

#endif // GUIBASE_RMDPTERCJQWN_H
