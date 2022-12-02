#ifndef TILEMAP_H
#define TILEMAP_H

#include "drawable.h"
#include "tmx.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>

namespace fck
{

class ResourceCache;

class TileMap : public Drawable
{
public:
    static std::vector<TileMap *> createFromTmx(Tmx *tmx, int32_t layer_id = -1);

    TileMap();
    TileMap(
        sf::Texture *texture,
        const sf::Vector2i &map_size,
        const sf::Vector2i &tile_size,
        const std::vector<std::vector<int32_t>> &tiles = std::vector<std::vector<int32_t>>());
    ~TileMap() = default;

    sf::Texture *texture() const;
    void setTexture(sf::Texture *texture, const sf::Vector2i &tile_size = sf::Vector2i());

    const sf::Color &color() const;
    void setColor(const sf::Color &color);

    sf::Vector2i mapSize() const;
    void setMapSize(const sf::Vector2i &map_size);

    sf::Vector2i tileSize() const;
    void setTileSize(const sf::Vector2i &tile_size);

    void setTile(const sf::Vector2i &position, int32_t tile);
    void setTiles(const std::vector<std::vector<int32_t>> &tiles);

    sf::FloatRect localBounds() const;
    sf::FloatRect contentBounds() const;
    sf::Vector2f center() const;

protected:
    void draw(sf::RenderTarget &target, const sf::RenderStates &states);

private:
    void updatePositions();
    void updateTexCoords();

private:
    sf::VertexArray m_vertices;
    sf::Texture *m_texture;

    sf::Vector2i m_map_size;
    sf::Vector2i m_tile_size;

    std::vector<std::vector<int32_t>> m_tiles;
};

} // namespace fck

#endif // TILEMAP_H
