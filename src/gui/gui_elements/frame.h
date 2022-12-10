#ifndef FRAME_EKBGLLBRCXRS_H
#define FRAME_EKBGLLBRCXRS_H

#include "../../fck/b2_dynamic_tree.h"
#include "../../fck/sigslot.h"

#include <SFML/Graphics.hpp>

#include <vector>

namespace fck
{
class GuiBase;
}

namespace fck::gui
{

class ProgressBar;
class Minimap;
class Button;
class QuestionDialog;

class Frame : public sf::Drawable, public sf::Transformable
{
    friend class fck::GuiBase;

    friend class ProgressBar;
    friend class Minimap;
    friend class Button;
    friend class QuestionDialog;

public:
    Frame();
    Frame(sf::Texture &texture, const sf::IntRect &frame_texture_rect);
    ~Frame();

    void setPosition(const sf::Vector2f &position);
    void setRotation(sf::Angle angle);
    void setScale(const sf::Vector2f &factors);
    void setOrigin(const sf::Vector2f &origin);

    void move(const sf::Vector2f &offset);
    void rotate(sf::Angle angle);
    void scale(const sf::Vector2f &factor);

    sf::Texture *getTexture() const;
    virtual void setTexture(sf::Texture &texture);

    const sf::Vector2f &getFrameSize() const;
    void setFrameSize(const sf::Vector2f &frame_size);

    const sf::Vector2i &getBorderSize() const;
    void setBorderSize(const sf::Vector2i &border_size);

    const sf::IntRect &getFrameTextureRect() const;
    void setFrameTextureRect(const sf::IntRect &frame_texture_rect);

    const sf::Color &getFrameColor() const;
    void setFrameColor(const sf::Color &color);

    virtual sf::FloatRect getLocalBounds() const;
    virtual sf::FloatRect getGlobalBounds() const;

    bool isVisible() const;
    void setVisible(bool visible);

    bool isEnable() const;
    void setEnable(bool enable);

    bool isHovered() const;
    bool isFocused() const;

    bool isFocusable() const;
    void setFocusable(bool focusabel);

    void setParent(Frame *frame);
    const sf::Transform &getParentsTransform() const;

protected:
    virtual void onHoverIn();
    virtual void onHoverOut();

    virtual void onFocusIn();
    virtual void onFocusOut();

    virtual void onMousePressed();
    virtual void onMouseReleased();

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updatePositions();
    void updateTexCoords();

    void updateFramesTree(const sf::Vector2f &offset);
    void updateParentsTransform();

private:
    sf::Texture *m_texture;

    sf::Vector2f m_frame_size;
    sf::Vector2i m_border_size;
    sf::VertexArray m_vertices;
    sf::IntRect m_frame_texture_rect;

    bool m_visible;
    bool m_enable;
    bool m_hovered;
    bool m_focused;
    bool m_focusable;

    Frame *m_parent;
    std::vector<Frame *> m_parents;
    std::vector<Frame *> m_children;
    sf::Transform m_parents_transform;

    int32_t m_proxy_id;
    fck::GuiBase *m_gui_base;
    int32_t m_z_order;
};

} // namespace fck::gui

#endif // FRAME_EKBGLLBRCXRS_H
