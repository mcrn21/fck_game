#include "tmx.h"
#include "base_game.h"
#include "common.h"
#include "utilities.h"

#include <spdlog/spdlog.h>

namespace fck
{

Tmx::Tmx()
{
    clear();
}

std::string Tmx::version() const
{
    return m_version;
}

std::string Tmx::tiledVersion() const
{
    return m_tiled_version;
}

Tmx::Orientation Tmx::orientation() const
{
    return m_orientation;
}

Tmx::RenderOrder Tmx::renderOrder() const
{
    return m_render_order;
}

sf::Vector2i Tmx::size() const
{
    return m_size;
}

sf::Vector2i Tmx::tileSize() const
{
    return m_tile_size;
}

int32_t Tmx::infinite() const
{
    return m_infinite;
}

int32_t Tmx::nextLayerId() const
{
    return m_next_layer_id;
}

int32_t Tmx::nextObjectId() const
{
    return m_next_object_id;
}

const std::map<std::string, std::string> &Tmx::properties() const
{
    return m_properties;
}

const std::vector<Tmx::Tileset> &Tmx::tilesets() const
{
    return m_tilesets;
}

const std::vector<Tmx::Layer> &Tmx::layers() const
{
    return m_layers;
}

const std::vector<Tmx::ObjectGroup> &Tmx::objectGroups() const
{
    return m_object_groups;
}

const std::vector<Tmx::Group> &Tmx::groups() const
{
    return m_groups;
}

bool Tmx::loadFromFile(const std::string &filename)
{
    clear();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    if (!result)
    {
        spdlog::warn("Tmx loading error: {}: {}", filename, result.description());
        return false;
    }

    try
    {
        loadXml(doc);
        return true;
    }
    catch (const std::exception &e)
    {
        clear();
        spdlog::warn("Tmx loading error: {}: {}", filename, e.what());
        return false;
    }
}

bool Tmx::loadFromMemory(const void *data, size_t size)
{
    clear();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(data, size);

    if (!result)
    {
        spdlog::warn("Tmx loading error: {}", result.description());
        return false;
    }

    try
    {
        loadXml(doc);
        return true;
    }
    catch (const std::exception &e)
    {
        clear();
        spdlog::warn("Tmx loading error: {}", e.what());
        return false;
    }
}

bool Tmx::loadFromStream(sf::InputStream &stream)
{
    clear();
    std::vector<char> data;
    stream.seek(0);

    const int64_t block_size = 1024;
    char block[block_size];

    while (stream.tell() < stream.getSize())
    {
        int64_t readed_block_size = stream.read(&block, block_size);
        int64_t old_data_size = data.size();
        data.resize(data.size() + readed_block_size);
        std::memcpy(&data.data()[old_data_size], &block[0], readed_block_size);
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer((const char *)(data.data()), data.size());

    if (!result)
    {
        spdlog::warn("Tmx loading error: {}", result.description());
        return false;
    }

    try
    {
        loadXml(doc);
        return true;
    }
    catch (const std::exception &e)
    {
        clear();
        spdlog::warn("Tmx loading error: {}", e.what());
        return false;
    }
}

void Tmx::clear()
{
    m_version = std::string();
    m_tiled_version = std::string();
    m_orientation = ORTHOGONAL;
    m_render_order = RIGHT_DOWN;
    m_size = sf::Vector2i();
    m_tile_size = sf::Vector2i();
    m_infinite = 0;
    m_next_layer_id = 0;
    m_next_object_id = 0;
    m_properties.clear();
    m_tilesets.clear();
    m_layers.clear();
    m_object_groups.clear();
    m_groups.clear();
}

void Tmx::loadXml(const pugi::xml_document &xml)
{
    pugi::xml_node map_node = xml.child("map");
    if (map_node.empty())
        throw Exception{"Node \"map\" not found"};

    if (!map_node.attribute("version").empty())
        m_version = map_node.attribute("version").as_string();

    if (!map_node.attribute("tiledversion").empty())
        m_tiled_version = map_node.attribute("tiledversion").as_string();

    if (!map_node.attribute("orientation").empty())
    {
        static std::map<std::string, Orientation> orientations = {{"orthogonal", ORTHOGONAL}};
        auto orientation_found = orientations.find(map_node.attribute("orientation").as_string());
        if (orientation_found != orientations.end())
            m_orientation = orientation_found->second;
    }

    if (!map_node.attribute("renderorder").empty())
    {
        static std::map<std::string, RenderOrder> render_orders = {{"right-down", RIGHT_DOWN}};
        auto render_order_found = render_orders.find(map_node.attribute("renderorder").as_string());
        if (render_order_found != render_orders.end())
            m_render_order = render_order_found->second;
    }

    if (map_node.attribute("width").empty())
        throw Exception{"\"width\" attribute for \"map\" element not found"};
    m_size.x = map_node.attribute("width").as_int();

    if (map_node.attribute("height").empty())
        throw Exception{"\"height\" attribute for \"map\" element not found"};
    m_size.y = map_node.attribute("height").as_int();

    if (map_node.attribute("tilewidth").empty())
        throw Exception{"\"tilewidth\" attribute for \"map\" element not found"};
    m_tile_size.x = map_node.attribute("tilewidth").as_int();

    if (map_node.attribute("tileheight").empty())
        throw Exception{"\"tileheight\" attribute for \"map\" element not found"};
    m_tile_size.y = map_node.attribute("tileheight").as_int();

    if (!map_node.attribute("infinite").empty())
        m_infinite = map_node.attribute("infinite").as_int();

    if (!map_node.attribute("nextlayerid").empty())
        m_next_layer_id = map_node.attribute("nextlayerid").as_int();

    if (!map_node.attribute("nextobjectid").empty())
        m_next_object_id = map_node.attribute("nextobjectid").as_int();

    m_properties = loadProperties(map_node);

    m_tilesets = loadTilesets(map_node);
    m_layers = loadLayers(map_node);
    m_object_groups = loadObjectGroups(map_node);
    m_groups = loadGroups(map_node);
}

std::vector<Tmx::Tileset> Tmx::loadTilesets(const pugi::xml_node &node)
{
    std::vector<Tileset> tilesets;

    pugi::xpath_node_set tileset_nodes = node.select_nodes("tileset");
    for (pugi::xpath_node tileset_xpath_node : tileset_nodes)
    {
        pugi::xml_node tileset_node = tileset_xpath_node.node();

        Tileset tileset;

        if (!tileset_node.attribute("name").empty())
            tileset.name = tileset_node.attribute("name").as_string();

        if (tileset_node.attribute("firstgid").empty())
            throw Exception{"\"firstgid\" attribute for \"tileset\" element not found"};
        tileset.first_gid = tileset_node.attribute("firstgid").as_int();

        if (tileset_node.attribute("tilewidth").empty())
            throw Exception{"\"tilewidth\" attribute for \"tileset\" element not found"};
        tileset.tile_size.x = tileset_node.attribute("tilewidth").as_int();

        if (tileset_node.attribute("tileheight").empty())
            throw Exception{"\"tileheight\" attribute for \"tileset\" element not found"};
        tileset.tile_size.y = tileset_node.attribute("tileheight").as_int();

        if (tileset_node.attribute("tilecount").empty())
            throw Exception{"\"tilecount\" attribute for \"tileset\" element not found"};
        tileset.tile_count = tileset_node.attribute("tilecount").as_int();

        if (tileset_node.attribute("columns").empty())
            throw Exception{"\"columns\" attribute for \"tileset\" element not found"};
        tileset.columns = tileset_node.attribute("columns").as_int();

        tileset.properties = loadProperties(tileset_node);

        tilesets.push_back(tileset);
    }

    return tilesets;
}

std::vector<Tmx::Layer> Tmx::loadLayers(const pugi::xml_node &node)
{
    std::vector<Layer> layers;

    pugi::xpath_node_set layers_nodes = node.select_nodes("layer");
    for (pugi::xpath_node layer_xpath_node : layers_nodes)
    {
        pugi::xml_node layer_node = layer_xpath_node.node();

        Layer layer;

        if (layer_node.attribute("id").empty())
            throw Exception{"\"id\" attribute for \"layer\" element not found"};
        layer.id = layer_node.attribute("id").as_int();

        if (!layer_node.attribute("name").empty())
            layer.name = layer_node.attribute("name").as_string();

        if (layer_node.attribute("width").empty())
            throw Exception{"\"width\" attribute for \"layer\" element not found"};
        layer.size.x = layer_node.attribute("width").as_int();

        if (layer_node.attribute("height").empty())
            throw Exception{"\"height\" attribute for \"layer\" element not found"};
        layer.size.y = layer_node.attribute("height").as_int();

        layer.properties = loadProperties(layer_node);

        if (layer_node.child("data").empty())
            throw Exception{"\"data\" attribute for \"layer\" element not found"};

        std::string data_str = layer_node.child_value("data");
        std::vector<std::string> data_strs = string::split(data_str, ',');

        int32_t x = 0;
        int32_t y = 0;
        layer.tiles.push_back(std::vector<int32_t>());
        layer.tiles.back().resize(layer.size.x, 0);
        for (std::string data : data_strs)
        {
            string::trim(data);
            layer.tiles[y][x] = std::stoi(data);
            ++x;
            if (x == layer.size.x && y < layer.size.y)
            {
                ++y;
                x = 0;
                if (y < layer.size.y)
                {
                    layer.tiles.push_back(std::vector<int32_t>());
                    layer.tiles.back().resize(layer.size.x, 0);
                }
            }
        }

        layers.push_back(layer);
    }

    return layers;
}

std::vector<Tmx::ObjectGroup> Tmx::loadObjectGroups(const pugi::xml_node &node)
{
    std::vector<ObjectGroup> object_groups;

    pugi::xpath_node_set object_group_nodes = node.select_nodes("objectgroup");
    for (pugi::xpath_node object_group_xpath_node : object_group_nodes)
    {
        pugi::xml_node object_group_node = object_group_xpath_node.node();

        ObjectGroup object_group;

        if (object_group_node.attribute("id").empty())
            throw Exception{"\"id\" attribute for \"objectgroup\" element not found"};
        object_group.id = object_group_node.attribute("id").as_int();

        if (!object_group_node.attribute("name").empty())
            object_group.name = object_group_node.attribute("name").as_string();

        pugi::xpath_node_set object_nodes = object_group_node.select_nodes("object");
        for (pugi::xpath_node object_xpath_node : object_nodes)
        {
            pugi::xml_node object_node = object_xpath_node.node();

            Object object;
            object.tile_gid = -1;
            object.type = Object::RECT;

            if (object_node.attribute("id").empty())
                throw Exception{"\"id\" attribute for \"object\" element not found"};
            object.id = object_node.attribute("id").as_int();

            if (!object_node.attribute("name").empty())
                object.name = object_node.attribute("name").as_string();

            if (object_node.attribute("x").empty())
                throw Exception{"\"x\" attribute for \"object\" element not found"};
            object.rect.left = object_node.attribute("x").as_int();

            if (object_node.attribute("y").empty())
                throw Exception{"\"y\" attribute for \"object\" element not found"};
            object.rect.top = object_node.attribute("y").as_int();

            if (!object_node.attribute("width").empty())
                object.rect.width = object_node.attribute("width").as_int();

            if (!object_node.attribute("height").empty())
                object.rect.height = object_node.attribute("height").as_int();

            if (!object_node.attribute("type").empty())
                object.type_string = object_node.attribute("type").as_string();

            if (!object_node.attribute("gid").empty())
            {
                object.tile_gid = object_node.attribute("gid").as_int();
                object.type = Object::TILE;
            }
            else if (!object_node.child("point").empty())
            {
                object.type = Object::POINT;
            }
            else if (!object_node.child("ellipse").empty())
            {
                object.type = Object::ELLIPSE;
            }
            else if (!object_node.child("polygon").empty())
            {
                std::string points_string
                    = object_node.child("polygon").attribute("points").as_string();
                std::vector<std::string> points = string::split(points_string, ' ');

                for (const auto &point_string : points)
                {
                    std::vector<std::string> point = string::split(point_string, ',');
                    if (point.size() < 2)
                        throw Exception{"invalid \"points\" for \"polygon\" element"};
                    object.polygon_points.push_back({std::stoi(point[0]), std::stoi(point[1])});
                }

                object.type = Object::POLYGON;
            }
            else if (!object_node.child("text").empty())
            {
                if (!object_node.child("text").attribute("wrap").empty())
                    object.text_wrap = object_node.child("text").attribute("wrap").as_int();
                object.text = object_node.child_value("text");
                object.type = Object::TEXT;
            }

            object_group.objects.push_back(object);
        }

        object_groups.push_back(object_group);
    }

    return object_groups;
}

std::vector<Tmx::Group> Tmx::loadGroups(const pugi::xml_node &node)
{
    std::vector<Group> groups;

    pugi::xpath_node_set group_nodes = node.select_nodes("group");
    for (pugi::xpath_node group_xpath_node : group_nodes)
    {
        pugi::xml_node group_node = group_xpath_node.node();

        Group group;

        if (group_node.attribute("id").empty())
            throw Exception{"\"id\" attribute for \"group\" element not found"};
        group.id = group_node.attribute("id").as_int();

        if (!group_node.attribute("name").empty())
            group.name = group_node.attribute("name").as_string();

        group.properties = loadProperties(group_node);

        group.layers = loadLayers(group_node);
        group.object_groups = loadObjectGroups(group_node);
        group.groups = loadGroups(group_node);

        groups.push_back(group);
    }

    return groups;
}

std::map<std::string, std::string> Tmx::loadProperties(const pugi::xml_node &node)
{
    std::map<std::string, std::string> properties;

    pugi::xpath_node_set property_nodes = node.select_nodes("properties/property");
    for (pugi::xpath_node propery_xpath_node : property_nodes)
    {
        pugi::xml_node property_node = propery_xpath_node.node();

        std::string proprty_name;
        if (property_node.attribute("name").empty())
            throw Exception{"\"name\" attribute for \"property\" element not found"};
        proprty_name = property_node.attribute("name").as_string();

        std::string property_value;
        if (property_node.attribute("value").empty())
            throw Exception{"\"value\" attribute for \"property\" element not found"};
        property_value = property_node.attribute("value").as_string();

        properties.emplace(proprty_name, property_value);
    }

    return properties;
}

} // namespace fck
