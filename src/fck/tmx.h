#ifndef TMX_H
#define TMX_H

#include <tinyxml2.h>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>
#include <vector>

namespace fck
{

class Tmx
{
public:
    enum Orientation
    {
        ORTHOGONAL
    };

    enum RenderOrder
    {
        RIGHT_DOWN
    };

    struct Tileset
    {
        int32_t first_gid = 0;
        std::string name;
        sf::Vector2i tile_size;
        int32_t tile_count = 0;
        int32_t columns = 0;
        std::map<std::string, std::string> properties;
    };

    struct Layer
    {
        int32_t id;
        std::string name;
        sf::Vector2i size;
        std::vector<std::vector<int32_t>> tiles;
        std::map<std::string, std::string> properties;
    };

    struct Object
    {
        enum Type
        {
            RECT,
            POINT,
            ELLIPSE,
            POLYGON,
            TILE,
            TEXT
        };

        int32_t id;
        std::string name;
        std::string type_string;
        sf::IntRect rect;
        Type type;
        int32_t tile_gid;
        std::vector<sf::Vector2i> polygon_points;
        int32_t text_wrap;
        std::string text;
    };

    struct ObjectGroup
    {
        int32_t id;
        std::string name;
        std::vector<Object> objects;
    };

    Tmx();
    Tmx(const Tmx &other);
    ~Tmx() = default;

    std::string version() const;
    std::string tiledVersion() const;
    Orientation orientation() const;
    RenderOrder renderOrder() const;
    sf::Vector2i size() const;
    sf::Vector2i tileSize() const;
    int32_t infinite() const;
    int32_t nextLayerId() const;
    int32_t nextObjectId() const;
    const std::vector<Tileset> &tilesets() const;
    const std::vector<Layer> &layers() const;
    const std::vector<ObjectGroup> &objectGroups() const;

    bool loadFromFile(const std::string &filename);
    bool loadFromMemory(const void *data, std::size_t size);
    bool loadFromStream(sf::InputStream &stream);

private:
    void clear();
    std::pair<bool, std::string> loadXml(const tinyxml2::XMLDocument &xml);

private:
    std::string m_version;
    std::string m_tiled_version;
    Orientation m_orientation;
    RenderOrder m_render_order;
    sf::Vector2i m_size;
    sf::Vector2i m_tile_size;
    int32_t m_infinite;
    int32_t m_next_layer_id;
    int32_t m_next_object_id;
    std::vector<Tileset> m_tilesets;
    std::vector<Layer> m_layers;
    std::vector<ObjectGroup> m_object_groups;
};
} // namespace fck

#endif // TMX_H
