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

bool Tmx::loadFromFile(const std::string &filename)
{
    clear();
    tinyxml2::XMLDocument xml;
    auto result = xml.LoadFile(filename.c_str());
    if (result != tinyxml2::XML_SUCCESS)
    {
        spdlog::warn("Tmx loading error: {0}", int32_t(result));
    }
    auto load_xml_result = loadXml(xml);
    if (!load_xml_result.first)
    {
        spdlog::warn("Tmx loading error: {0}", load_xml_result.second);
    }
    return load_xml_result.first;
}

bool Tmx::loadFromMemory(const void *data, size_t size)
{
    clear();
    tinyxml2::XMLDocument xml;
    auto result = xml.Parse((const char *)(data), size);
    if (result != tinyxml2::XML_SUCCESS)
    {
        spdlog::warn("Tmx loading error: {0}", int32_t(result));
    }
    auto load_xml_result = loadXml(xml);
    if (!load_xml_result.first)
    {
        spdlog::warn("Tmx loading error: {0}", load_xml_result.second);
    }
    return load_xml_result.first;
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

    tinyxml2::XMLDocument xml;
    auto result = xml.Parse((const char *)(data.data()), data.size());
    if (result != tinyxml2::XML_SUCCESS)
    {
        spdlog::warn("Tmx loading error: {0}", int32_t(result));
    }
    auto load_xml_result = loadXml(xml);
    if (!load_xml_result.first)
    {
        spdlog::warn("Tmx loading error: {0}", load_xml_result.second);
    }
    return load_xml_result.first;
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
    m_tilesets.clear();
    m_layers.clear();
}

std::pair<bool, std::string> Tmx::loadXml(const tinyxml2::XMLDocument &xml)
{
    auto map_alement = xml.FirstChildElement("map");
    if (!map_alement)
    {
        return std::make_pair(false, "\"map\" element not found!");
    }

    std::string version;
    std::string tiled_version;
    Orientation orientation = ORTHOGONAL;
    RenderOrder render_order = RIGHT_DOWN;
    sf::Vector2i size;
    sf::Vector2i tile_size;
    int32_t infinite = 0;
    int32_t next_layer_id = 0;
    int32_t next_object_id = 0;
    std::vector<Tileset> tilesets;
    std::vector<Layer> layers;
    std::vector<ObjectGroup> object_groups;

    if (map_alement->Attribute("version"))
        version = map_alement->Attribute("version");

    if (map_alement->Attribute("tiledversion"))
        version = map_alement->Attribute("tiledversion");

    if (map_alement->Attribute("orientation"))
    {
        static std::map<std::string, Orientation> orientations = {{"orthogonal", ORTHOGONAL}};
        auto orientation_found = orientations.find(map_alement->Attribute("orientation"));
        if (orientation_found != orientations.end())
            orientation = orientation_found->second;
    }

    if (map_alement->Attribute("renderorder"))
    {
        static std::map<std::string, RenderOrder> render_orders = {{"right-down", RIGHT_DOWN}};
        auto render_order_found = render_orders.find(map_alement->Attribute("renderorder"));
        if (render_order_found != render_orders.end())
            render_order = render_order_found->second;
    }

    if (!map_alement->Attribute("width"))
        return std::make_pair(false, "\"width\" attribute for \"map\" element not found!");
    size.x = map_alement->IntAttribute("width");

    if (!map_alement->Attribute("height"))
        return std::make_pair(false, "\"height\" attribute for \"map\" element not found!");
    size.y = map_alement->IntAttribute("height");

    if (!map_alement->Attribute("tilewidth"))
        return std::make_pair(false, "\"tilewidth\" attribute for \"map\" element not found!");
    tile_size.x = map_alement->IntAttribute("tilewidth");

    if (!map_alement->Attribute("tileheight"))
        return std::make_pair(false, "\"tileheight\" attribute for \"map\" element not found!");
    tile_size.y = map_alement->IntAttribute("tileheight");

    if (!map_alement->Attribute("infinite"))
        return std::make_pair(false, "\"infinite\" attribute for \"map\" element not found!");
    infinite = map_alement->IntAttribute("infinite");

    if (!map_alement->Attribute("nextlayerid"))
        return std::make_pair(false, "\"nextlayerid\" attribute for \"map\" element not found!");
    next_layer_id = map_alement->IntAttribute("nextlayerid");

    if (!map_alement->Attribute("nextobjectid"))
        return std::make_pair(false, "\"nextobjectid\" attribute for \"map\" element not found!");
    next_object_id = map_alement->IntAttribute("nextobjectid");

    // Tilesets
    auto tileset_element = map_alement->FirstChildElement("tileset");
    while (tileset_element)
    {
        Tileset tileset;

        if (!tileset_element->Attribute("firstgid"))
            return std::make_pair(
                false, "\"firstgid\" attribute for \"tileset\" element not found!");
        tileset.first_gid = tileset_element->IntAttribute("firstgid");

        if (tileset_element->Attribute("name"))
            tileset.name = tileset_element->Attribute("name");

        if (!tileset_element->Attribute("tilewidth"))
            return std::make_pair(
                false, "\"tilewidth\" attribute for \"tileset\" element not found!");
        tileset.tile_size.x = tileset_element->IntAttribute("tilewidth");

        if (!tileset_element->Attribute("tileheight"))
            return std::make_pair(
                false, "\"tileheight\" attribute for \"tileset\" element not found!");
        tileset.tile_size.x = tileset_element->IntAttribute("tileheight");

        if (!tileset_element->Attribute("tilecount"))
            return std::make_pair(
                false, "\"tilecount\" attribute for \"tileset\" element not found!");
        tileset.tile_count = tileset_element->IntAttribute("tilecount");

        if (!tileset_element->Attribute("columns"))
            return std::make_pair(
                false, "\"columns\" attribute for \"tileset\" element not found!");
        tileset.columns = tileset_element->IntAttribute("columns");

        auto properties_element = tileset_element->FirstChildElement("properties");
        if (properties_element)
        {
            auto property_element = properties_element->FirstChildElement("property");
            while (property_element)
            {
                std::string proprty_name;
                if (!property_element->Attribute("name"))
                    return std::make_pair(
                        false, "\"name\" attribute for \"property\" element not found!");
                proprty_name = property_element->Attribute("name");

                std::string property_value;
                if (!property_element->Attribute("value"))
                    return std::make_pair(
                        false, "\"value\" attribute for \"property\" element not found!");
                property_value = property_element->Attribute("value");

                tileset.properties.emplace(proprty_name, property_value);

                property_element = property_element->NextSiblingElement("property");
            }
        }

        //        auto image_element = tileset_element->FirstChildElement("image");
        //        if (!image_element)
        //            return std::make_pair(false, "\"image\" element for \"tileset\" element not found!");

        //        if (!image_element->Attribute("source"))
        //            return std::make_pair(false, "\"source\" attribute for \"image\" element not found!");
        //        tileset.image = Game::resource<sf::Texture>(image_element->Attribute("source"));

        tilesets.push_back(tileset);

        tileset_element = tileset_element->NextSiblingElement("tileset");
    }

    // Layers
    auto layer_element = map_alement->FirstChildElement("layer");
    while (layer_element)
    {
        Layer layer;

        if (!layer_element->Attribute("id"))
            return std::make_pair(false, "\"id\" attribute for \"layer\" element not found!");
        layer.id = layer_element->IntAttribute("id");

        if (layer_element->Attribute("name"))
            layer.name = layer_element->Attribute("name");

        if (!layer_element->Attribute("width"))
            return std::make_pair(false, "\"width\" attribute for \"layer\" element not found!");
        layer.size.x = layer_element->IntAttribute("width");

        if (!layer_element->Attribute("height"))
            return std::make_pair(false, "\"height\" attribute for \"layer\" element not found!");
        layer.size.y = layer_element->IntAttribute("height");

        auto properties_element = layer_element->FirstChildElement("properties");
        if (properties_element)
        {
            auto property_element = properties_element->FirstChildElement("property");
            while (property_element)
            {
                std::string proprty_name;
                if (!property_element->Attribute("name"))
                    return std::make_pair(
                        false, "\"name\" attribute for \"property\" element not found!");
                proprty_name = property_element->Attribute("name");

                std::string property_value;
                if (!property_element->Attribute("value"))
                    return std::make_pair(
                        false, "\"value\" attribute for \"property\" element not found!");
                property_value = property_element->Attribute("value");

                layer.properties.emplace(proprty_name, property_value);

                property_element = property_element->NextSiblingElement("property");
            }
        }

        auto data_element = layer_element->FirstChildElement("data");
        if (!data_element)
            return std::make_pair(false, "\"data\" element for \"layer\" element not found!");

        std::string data_str = data_element->GetText();
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
                layer.tiles.push_back(std::vector<int32_t>());
                layer.tiles.back().resize(layer.size.x, 0);
            }
        }

        layers.push_back(layer);

        layer_element = layer_element->NextSiblingElement("layer");
    }

    // Object groups
    auto objectgroup_element = map_alement->FirstChildElement("objectgroup");
    while (objectgroup_element)
    {
        ObjectGroup object_group;

        if (!objectgroup_element->Attribute("id"))
            return std::make_pair(false, "\"id\" attribute for \"objectgroup\" element not found!");
        object_group.id = objectgroup_element->IntAttribute("id");

        if (objectgroup_element->Attribute("name"))
            object_group.name = objectgroup_element->Attribute("name");

        auto object_element = objectgroup_element->FirstChildElement("object");
        while (object_element)
        {
            Object object;

            object.tile_gid = -1;
            object.type = Object::RECT;

            if (!object_element->Attribute("id"))
                return std::make_pair(false, "\"id\" attribute for \"object\" element not found!");
            object.id = object_element->IntAttribute("id");

            if (object_element->Attribute("name"))
                object.name = object_element->Attribute("name");

            if (!object_element->Attribute("x"))
                return std::make_pair(false, "\"x\" attribute for \"object\" element not found!");
            object.rect.left = object_element->IntAttribute("x");

            if (!object_element->Attribute("y"))
                return std::make_pair(false, "\"y\" attribute for \"object\" element not found!");
            object.rect.top = object_element->IntAttribute("y");

            if (object_element->Attribute("width"))
                object.rect.width = object_element->IntAttribute("width");

            if (object_element->Attribute("height"))
                object.rect.height = object_element->IntAttribute("height");

            if (object_element->Attribute("type"))
                object.type_string = object_element->Attribute("type");

            if (object_element->Attribute("gid"))
            {
                object.tile_gid = object_element->IntAttribute("gid");
                object.type = Object::TILE;
            }
            else if (object_element->FirstChildElement("point"))
            {
                object.type = Object::POINT;
            }
            else if (object_element->FirstChildElement("ellipse"))
            {
                object.type = Object::ELLIPSE;
            }
            else if (object_element->FirstChildElement("polygon"))
            {
                auto polygon_element = object_element->FirstChildElement("polygon");
                std::string points_string = polygon_element->Attribute("points");
                std::vector<std::string> points = string::split(points_string, ' ');

                for (const auto &point_string : points)
                {
                    std::vector<std::string> point = string::split(point_string, ',');
                    if (point.size() < 2)
                        return std::make_pair(false, "invalid \"points\" for \"polygon\" element!");
                    object.polygon_points.push_back({std::stoi(point[0]), std::stoi(point[1])});
                }

                object.type = Object::POLYGON;
            }
            else if (object_element->FirstChildElement("text"))
            {
                auto text_element = object_element->FirstChildElement("text");
                if (text_element->Attribute("wrap"))
                    object.text_wrap = text_element->IntAttribute("wrap");
                object.text = text_element->GetText();
                object.type = Object::TEXT;
            }

            std::sort(
                object_group.objects.begin(),
                object_group.objects.end(),
                [](const auto &a, const auto &b) { return a.id < b.id; });

            object_group.objects.push_back(object);

            object_element = object_element->NextSiblingElement("object");
        }

        object_groups.push_back(std::move(object_group));

        objectgroup_element = objectgroup_element->NextSiblingElement("objectgroup");
    }

    //    std::sort(tilesets.begin(), tilesets.end(), [](const auto &a, const auto &b) {
    //        return a.first_gid < b.first_gid;
    //    });
    //    std::sort(
    //        layers.begin(), layers.end(), [](const auto &a, const auto &b) { return a.id < b.id; });
    //    std::sort(object_groups.begin(), object_groups.end(), [](const auto &a, const auto &b) {
    //        return a.id < b.id;
    //    });

    m_version = version;
    m_tiled_version = tiled_version;
    m_orientation = orientation;
    m_render_order = render_order;
    m_size = size;
    m_tile_size = tile_size;
    m_infinite = infinite;
    m_next_layer_id = next_layer_id;
    m_next_object_id = next_object_id;
    m_tilesets = tilesets;
    m_layers = layers;
    m_object_groups = object_groups;

    return std::make_pair(true, std::string());
}

} // namespace fck
