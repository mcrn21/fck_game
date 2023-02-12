#ifndef MINIMAP_ESYFJTFZLGFX_H
#define MINIMAP_ESYFJTFZLGFX_H

#include "box.h"
#include "grid.h"
#include "widget.h"

#include "../map/map.h"

#include <SFML/Graphics.hpp>

namespace fck::gui
{

class Minimap : public Widget
{
public:
    Minimap(Widget *parent = nullptr);
    ~Minimap() = default;

    const Vector2D<map::Chunk *> *getChunks() const;
    void setChunks(const Vector2D<map::Chunk *> &chunks);

    void setChunkOpened(const sf::Vector2i &chunk_coords);
    void setCurrentChunk(const sf::Vector2i &chunk_coords);

protected:
    void onResized(const sf::Vector2f &size) override;
    void onThemeChanged(const WidgetTheme &widget_theme) override;

private:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;

    void updateGeometry();

private:
    const Vector2D<map::Chunk *> *m_chunks;

    Box m_background;

    Grid m_chunks_grid;
    std::unordered_map<int32_t, int32_t> m_chunks_grid_texture_indexes;

    Grid m_chunks_type_grid;
    std::unordered_map<int32_t, int32_t> m_chunks_type_grid_texture_indexes;

    Box m_current_chunk_highlight;

    sf::Vector2i m_current_chunk;
};

} // namespace fck::gui

#endif // MINIMAP_ESYFJTFZLGFX_H
