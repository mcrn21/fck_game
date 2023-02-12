#include "minimap.h"

#include "../fck/clipping.h"
#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck::gui
{

Minimap::Minimap(Widget *parent) : Widget{parent}
{
    setWidgetTheme(WidgetTheme::get<Minimap>());
}

const Vector2D<map::Chunk *> *Minimap::getChunks() const
{
    return m_chunks;
}

void Minimap::setChunks(const Vector2D<map::Chunk *> &chunks)
{
    m_chunks = &chunks;
    m_chunks_grid.setSize(m_chunks->getSize2D());
    m_chunks_type_grid.setSize(m_chunks->getSize2D());

    for (int32_t i = 0; i < m_chunks->getSize(); ++i)
    {
        auto coord = m_chunks->transformIndex(i);
        m_chunks_grid.setCellTexture(
            coord,
            m_chunks->at(i)
                ? (m_chunks->at(i)->isOpen()
                       ? m_chunks_grid_texture_indexes[m_chunks->at(i)->getNeighbors()]
                       : -1)
                : -1);

        m_chunks_type_grid.setCellTexture(
            coord,
            m_chunks->at(i)
                ? (m_chunks->at(i)->isOpen()
                       ? m_chunks_type_grid_texture_indexes[m_chunks->at(i)->getType()]
                       : -1)
                : -1);
    }
}

void Minimap::setChunkOpened(const sf::Vector2i &chunk_coords)
{
    if (!m_chunks)
        return;

    m_chunks_grid.setCellTexture(
        chunk_coords,
        m_chunks_grid_texture_indexes[m_chunks->getData(chunk_coords)->getNeighbors()]);

    m_chunks_type_grid.setCellTexture(
        chunk_coords,
        m_chunks_type_grid_texture_indexes[m_chunks->getData(chunk_coords)->getType()]);
}

void Minimap::setCurrentChunk(const sf::Vector2i &chunk_coords)
{
    m_current_chunk = chunk_coords;
    updateGeometry();
}

void Minimap::onResized(const sf::Vector2f &size)
{
    m_background.setSize(size);
    updateGeometry();
}

void Minimap::onThemeChanged(const WidgetTheme &widget_theme)
{
    m_background.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_background.setBorderSize(widget_theme.background.border_size);
    m_background.setTextureRect(widget_theme.background.texture_rects.at(getState()));
    m_background.setBorderTextureSize(widget_theme.background.texture_border_size);

    sf::Vector2f cell_size = {34.0f, 34.0f};

    m_chunks_grid.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_chunks_grid.setTextureRect({{128, 0}, {96, 192}});
    m_chunks_grid.setTextureCellSize({32, 32});
    m_chunks_grid.setCellSize(cell_size);

    m_chunks_grid_texture_indexes
        = {{0, 15},
           {chunk_side::LEFT, 0},
           {chunk_side::LEFT | chunk_side::TOP, 1},
           {chunk_side::LEFT | chunk_side::TOP | chunk_side::RIGHT, 2},
           {chunk_side::TOP, 3},
           {chunk_side::TOP | chunk_side::RIGHT, 4},
           {chunk_side::TOP | chunk_side::RIGHT | chunk_side::BOTTOM, 5},
           {chunk_side::RIGHT, 6},
           {chunk_side::RIGHT | chunk_side::BOTTOM, 7},
           {chunk_side::RIGHT | chunk_side::BOTTOM | chunk_side::LEFT, 8},
           {chunk_side::BOTTOM, 9},
           {chunk_side::BOTTOM | chunk_side::LEFT, 10},
           {chunk_side::BOTTOM | chunk_side::LEFT | chunk_side::TOP, 11},
           {chunk_side::TOP | chunk_side::BOTTOM, 12},
           {chunk_side::LEFT | chunk_side::RIGHT, 13},
           {chunk_side::LEFT | chunk_side::TOP | chunk_side::RIGHT | chunk_side::BOTTOM, 14}};

    m_chunks_type_grid.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_chunks_type_grid.setTextureRect({{128, 192}, {96, 32}});
    m_chunks_type_grid.setTextureCellSize({32, 32});
    m_chunks_type_grid.setCellSize(cell_size);

    m_chunks_type_grid_texture_indexes
        = {{chunk_type::UNKNOW, 0},
           {chunk_type::DEFAULT, -1},
           {chunk_type::BOSS, 1},
           {chunk_type::TRADER, 2}};

    m_current_chunk_highlight.setTexture(*ResourceCache::get<sf::Texture>(widget_theme.texture));
    m_current_chunk_highlight.setBorderSize(0.0f);
    m_current_chunk_highlight.setTextureRect({{160, 160}, {32, 32}});
    m_current_chunk_highlight.setSize(cell_size);
    m_current_chunk_highlight.setBorderTextureSize(0);

    updateGeometry();
}

void Minimap::draw(sf::RenderTarget &target, const sf::RenderStates &states) const
{
    sf::RenderStates new_states = states;
    new_states.transform *= getTransform();

    target.draw(m_background, new_states);

    {
        const WidgetTheme &widget_theme = getWidgetTheme();
        Clipping clipping(
            target,
            new_states,
            {widget_theme.padding.left, widget_theme.padding.top},
            getLocalBounds().getSize()
                - sf::Vector2f{
                    widget_theme.padding.left + widget_theme.padding.right,
                    widget_theme.padding.top + widget_theme.padding.bottom});

        target.draw(m_chunks_grid, new_states);
        target.draw(m_chunks_type_grid, new_states);
    }

    target.draw(m_current_chunk_highlight, new_states);
}

void Minimap::updateGeometry()
{
    sf::Vector2f position = getSize() / 2.0f
        - (vector2::mult(sf::Vector2f{m_current_chunk}, m_chunks_grid.getCellSize())
           + m_chunks_grid.getCellSize() / 2.0f);

    m_chunks_grid.setPoisition(position);
    m_chunks_type_grid.setPoisition(position);

    m_current_chunk_highlight.setPoisition(
        (getSize() - m_current_chunk_highlight.getSize()) / 2.0f);
}

} // namespace fck::gui
