#include "gui_style.h"

#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck::gui
{

std::string GuiStyle::ui_texture_name = "ui";
std::string GuiStyle::ui_font_name = "mini_pixel-7";

sf::Vector2f GuiStyle::viewport_offset = {20.0f, 40.0f};
sf::Vector2f GuiStyle::stats_scale = {4.0f, 4.0f};

sf::IntRect GuiStyle::frame_texture_rect = {{0, 0}, {40, 8}};
sf::IntRect GuiStyle::frame_transparented_texture_rect = {{0, 8}, {40, 8}};
sf::Vector2i GuiStyle::frame_border_size = {3, 3};

sf::IntRect GuiStyle::progress_bar_red_texture_rect = {{41, 2}, {1, 4}};
sf::IntRect GuiStyle::progress_bar_blue_texture_rect = {{43, 2}, {1, 4}};
sf::Vector2f GuiStyle::progress_bar_offset = {2.0f, 2.0f};

sf::Vector2f GuiStyle::hp_progress_bar_size = {64.0f, 8.0f};
sf::Vector2f GuiStyle::armor_progress_bar_size = {52.0f, 8.0f};
sf::Vector2f GuiStyle::stats_progress_bar_text_offset = {2.0f, -4.0f};
uint32_t GuiStyle::stats_font_size = 24;

std::unordered_map<int32_t, sf::IntRect> GuiStyle::minimap_room_texture_rects
    = {{0, {{1, 72}, {11, 11}}},
       {Level::Room::LEFT, {{1, 17}, {11, 11}}},
       {Level::Room::LEFT | Level::Room::TOP, {{12, 17}, {11, 11}}},
       {Level::Room::LEFT | Level::Room::TOP | Level::Room::RIGHT, {{23, 17}, {11, 11}}},
       {Level::Room::TOP, {{1, 28}, {11, 11}}},
       {Level::Room::TOP | Level::Room::RIGHT, {{12, 28}, {11, 11}}},
       {Level::Room::TOP | Level::Room::RIGHT | Level::Room::BOTTOM, {{23, 28}, {11, 11}}},
       {Level::Room::RIGHT, {{1, 39}, {11, 11}}},
       {Level::Room::RIGHT | Level::Room::BOTTOM, {{12, 39}, {11, 11}}},
       {Level::Room::RIGHT | Level::Room::BOTTOM | Level::Room::LEFT, {{23, 39}, {11, 11}}},
       {Level::Room::BOTTOM, {{1, 50}, {11, 11}}},
       {Level::Room::BOTTOM | Level::Room::LEFT, {{12, 50}, {11, 11}}},
       {Level::Room::BOTTOM | Level::Room::LEFT | Level::Room::TOP, {{23, 50}, {11, 11}}},
       {Level::Room::TOP | Level::Room::BOTTOM, {{1, 61}, {11, 11}}},
       {Level::Room::LEFT | Level::Room::RIGHT, {{12, 61}, {11, 11}}},
       {Level::Room::LEFT | Level::Room::TOP | Level::Room::RIGHT | Level::Room::BOTTOM,
        {{23, 61}, {11, 11}}}};

std::unordered_map<Level::Room::Type, sf::IntRect> GuiStyle::minimap_room_type_texture_rects
    = {{Level::Room::UNKNOW, {{1, 83}, {11, 11}}},
       {Level::Room::DEFAULT, {{0, 0}, {0, 0}}},
       {Level::Room::BOSS, {{12, 83}, {11, 11}}},
       {Level::Room::TRADER, {{23, 83}, {11, 11}}}};

sf::IntRect GuiStyle::minimap_current_room_texture_rect = {{12, 72}, {11, 11}};
sf::Vector2f GuiStyle::minimap_size = {30.0f, 30.0f};
sf::Vector2f GuiStyle::minimap_text_offset = {4.0f, -7.0f};

ProgressBar *GuiStyle::createPlayerHpBar()
{
    ProgressBar *progress_bar = new ProgressBar{
        *ResourceCache::resource<sf::Texture>(ui_texture_name),
        frame_texture_rect,
        progress_bar_red_texture_rect,
        progress_bar_offset};

    progress_bar->setBorderSize(frame_border_size);
    progress_bar->setFrameSize(hp_progress_bar_size);
    progress_bar->setScale(stats_scale);

    progress_bar->text().setFont(*ResourceCache::resource<sf::Font>(ui_font_name));
    progress_bar->text().setCharacterSize(stats_font_size);
    progress_bar->text().setOutlineColor(sf::Color::Black);
    progress_bar->text().setOutlineThickness(1.0f);
    progress_bar->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    progress_bar->text().setPosition(stats_progress_bar_text_offset);

    return progress_bar;
}

ProgressBar *GuiStyle::createPlayerArmorBar()
{
    ProgressBar *progress_bar = new ProgressBar{
        *ResourceCache::resource<sf::Texture>(ui_texture_name),
        frame_texture_rect,
        progress_bar_blue_texture_rect,
        progress_bar_offset};

    progress_bar->setBorderSize(frame_border_size);
    progress_bar->setFrameSize(armor_progress_bar_size);
    progress_bar->setScale(stats_scale);

    progress_bar->text().setFont(*ResourceCache::resource<sf::Font>(ui_font_name));
    progress_bar->text().setCharacterSize(stats_font_size);
    progress_bar->text().setOutlineColor(sf::Color::Black);
    progress_bar->text().setOutlineThickness(1.0f);
    progress_bar->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    progress_bar->text().setPosition(stats_progress_bar_text_offset);

    return progress_bar;
}

ProgressBar *GuiStyle::createTargetHpBar()
{
    ProgressBar *progress_bar = new ProgressBar{
        *ResourceCache::resource<sf::Texture>(ui_texture_name),
        frame_texture_rect,
        progress_bar_red_texture_rect,
        progress_bar_offset};

    progress_bar->setBorderSize(frame_border_size);
    progress_bar->setFrameSize(hp_progress_bar_size);
    progress_bar->setScale(vector2::mult(stats_scale, {-1.0f, 1.0f}));

    progress_bar->text().setFont(*ResourceCache::resource<sf::Font>(ui_font_name));
    progress_bar->text().setCharacterSize(stats_font_size);
    progress_bar->text().setOutlineColor(sf::Color::Black);
    progress_bar->text().setOutlineThickness(1.0f);
    progress_bar->text().setScale(vector2::div({-1.0f, 1.0f}, stats_scale));
    progress_bar->text().setPosition(stats_progress_bar_text_offset);

    return progress_bar;
}

Minimap *GuiStyle::createMinimap()
{
    Minimap *minimap = new Minimap{
        *ResourceCache::resource<sf::Texture>(ui_texture_name),
        frame_transparented_texture_rect,
        minimap_room_texture_rects,
        minimap_room_type_texture_rects,
        minimap_current_room_texture_rect};

    minimap->setBorderSize(frame_border_size);
    minimap->setFrameSize(minimap_size);
    minimap->setScale(stats_scale);

    minimap->text().setFont(*ResourceCache::resource<sf::Font>(ui_font_name));
    minimap->text().setCharacterSize(stats_font_size);
    minimap->text().setOutlineColor(sf::Color::Black);
    minimap->text().setOutlineThickness(1.0f);
    minimap->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    minimap->text().setPosition(
        {minimap_text_offset.x, minimap->getLocalBounds().height + minimap_text_offset.y});
    minimap->text().setString("M");

    return minimap;
}

} // namespace fck::gui
