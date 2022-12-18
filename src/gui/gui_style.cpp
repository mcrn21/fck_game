#include "gui_style.h"

#include "gui_elements/gui_elements.h"

#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck::gui
{

std::string Style::ui_texture_name = "ui";
std::string Style::ui_font_name = "mini_pixel-7";

sf::Vector2f Style::viewport_offset = {20.0f, 40.0f};
sf::Vector2f Style::stats_scale = {4.0f, 4.0f};

sf::IntRect Style::frame_texture_rect = {{0, 0}, {40, 8}};
sf::IntRect Style::frame_transparented_texture_rect = {{0, 8}, {40, 8}};
sf::Vector2i Style::frame_border_size = {3, 3};

sf::IntRect Style::progress_bar_red_texture_rect = {{41, 2}, {1, 4}};
sf::IntRect Style::progress_bar_blue_texture_rect = {{43, 2}, {1, 4}};
sf::IntRect Style::progress_bar_yellow_texture_rect = {{45, 2}, {1, 4}};
sf::Vector2f Style::progress_bar_offset = {2.0f, 2.0f};

sf::Vector2f Style::loading_progress_bar_size = {64.0f, 8.0f};
sf::Vector2f Style::loading_progress_bar_text_offset = {0.0f, -4.0f};
uint32_t Style::loading_progress_bar_font_size = 30;

sf::Color Style::main_menu_button_default_color = sf::Color(255, 255, 255);
sf::IntRect Style::main_menu_button_default_frame_texture_rect = {{0, 0}, {0, 0}};
sf::Color Style::main_menu_button_hover_color = sf::Color(255, 255, 255);
sf::IntRect Style::main_menu_button_hover_frame_texture_rect = {{8, 96}, {8, 8}};
sf::Color Style::main_menu_button_pressed_color = sf::Color(255, 255, 255);
sf::IntRect Style::main_menu_button_pressed_frame_texture_rect = {{8, 96}, {8, 8}};
uint32_t Style::main_menu_button_font_size = 30;

sf::IntRect Style::dialog_frame_texture_rect = {{16, 96}, {8, 8}};

sf::Vector2f Style::hp_progress_bar_size = {64.0f, 8.0f};
sf::Vector2f Style::armor_progress_bar_size = {52.0f, 8.0f};
sf::Vector2f Style::stats_progress_bar_text_offset = {2.0f, -4.0f};
uint32_t Style::stats_font_size = 24;

std::unordered_map<int32_t, sf::IntRect> Style::minimap_room_texture_rects
    = {{0, {{1, 72}, {11, 11}}},
       {room_side::LEFT, {{1, 17}, {11, 11}}},
       {room_side::LEFT | room_side::TOP, {{12, 17}, {11, 11}}},
       {room_side::LEFT | room_side::TOP | room_side::RIGHT, {{23, 17}, {11, 11}}},
       {room_side::TOP, {{1, 28}, {11, 11}}},
       {room_side::TOP | room_side::RIGHT, {{12, 28}, {11, 11}}},
       {room_side::TOP | room_side::RIGHT | room_side::BOTTOM, {{23, 28}, {11, 11}}},
       {room_side::RIGHT, {{1, 39}, {11, 11}}},
       {room_side::RIGHT | room_side::BOTTOM, {{12, 39}, {11, 11}}},
       {room_side::RIGHT | room_side::BOTTOM | room_side::LEFT, {{23, 39}, {11, 11}}},
       {room_side::BOTTOM, {{1, 50}, {11, 11}}},
       {room_side::BOTTOM | room_side::LEFT, {{12, 50}, {11, 11}}},
       {room_side::BOTTOM | room_side::LEFT | room_side::TOP, {{23, 50}, {11, 11}}},
       {room_side::TOP | room_side::BOTTOM, {{1, 61}, {11, 11}}},
       {room_side::LEFT | room_side::RIGHT, {{12, 61}, {11, 11}}},
       {room_side::LEFT | room_side::TOP | room_side::RIGHT | room_side::BOTTOM,
        {{23, 61}, {11, 11}}}};

std::unordered_map<room_type::Type, sf::IntRect> Style::minimap_room_type_texture_rects
    = {{room_type::UNKNOW, {{1, 83}, {11, 11}}},
       {room_type::DEFAULT, {{0, 0}, {0, 0}}},
       {room_type::BOSS, {{12, 83}, {11, 11}}},
       {room_type::TRADER, {{23, 83}, {11, 11}}}};

sf::IntRect Style::minimap_current_room_texture_rect = {{12, 72}, {11, 11}};
sf::Vector2f Style::minimap_size = {30.0f, 30.0f};
sf::Vector2f Style::minimap_text_offset = {4.0f, -7.0f};

Style Style::frame_style = []() {
    Style style;

    style.values["texture"] = std::string("ui");
    style.values["scale"] = sf::Vector2f{4.0f, 4.0f};
    style.values["frame_border_size"] = sf::Vector2i{3, 3};
    style.values["frame_default_texture_rect"] = sf::IntRect{{0, 0}, {40, 8}};
    style.values["frame_hovered_texture_rect"] = sf::IntRect{{0, 0}, {40, 8}};
    style.values["frame_pressed_texture_rect"] = sf::IntRect{{0, 0}, {40, 8}};
    style.values["frame_default_color"] = sf::Color::White;
    style.values["frame_hovered_color"] = sf::Color::White;
    style.values["frame_pressed_color"] = sf::Color::White;

    return style;
}();

ProgressBar *Style::createLoadingProgressBar()
{
    ProgressBar *progress_bar = new ProgressBar{
        *ResourceCache::getResource<sf::Texture>(ui_texture_name),
        frame_texture_rect,
        progress_bar_yellow_texture_rect,
        progress_bar_offset};

    progress_bar->setBorderSize(frame_border_size);
    progress_bar->setFrameSize(loading_progress_bar_size);
    progress_bar->setScale(stats_scale);

    progress_bar->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    progress_bar->text().setCharacterSize(loading_progress_bar_font_size);
    progress_bar->text().setOutlineColor(sf::Color::Black);
    progress_bar->text().setOutlineThickness(1.0f);
    progress_bar->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    progress_bar->text().setPosition(
        sf::Vector2f{progress_bar->getLocalBounds().width, 0.0f}
        + loading_progress_bar_text_offset);

    return progress_bar;
}

ProgressBar *Style::createPlayerHpBar()
{
    ProgressBar *progress_bar = new ProgressBar{
        *ResourceCache::getResource<sf::Texture>(ui_texture_name),
        frame_texture_rect,
        progress_bar_red_texture_rect,
        progress_bar_offset};

    progress_bar->setBorderSize(frame_border_size);
    progress_bar->setFrameSize(hp_progress_bar_size);
    progress_bar->setScale(stats_scale);

    progress_bar->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    progress_bar->text().setCharacterSize(stats_font_size);
    progress_bar->text().setOutlineColor(sf::Color::Black);
    progress_bar->text().setOutlineThickness(1.0f);
    progress_bar->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    progress_bar->text().setPosition(stats_progress_bar_text_offset);

    return progress_bar;
}

ProgressBar *Style::createPlayerArmorBar()
{
    ProgressBar *progress_bar = new ProgressBar{
        *ResourceCache::getResource<sf::Texture>(ui_texture_name),
        frame_texture_rect,
        progress_bar_blue_texture_rect,
        progress_bar_offset};

    progress_bar->setBorderSize(frame_border_size);
    progress_bar->setFrameSize(armor_progress_bar_size);
    progress_bar->setScale(stats_scale);

    progress_bar->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    progress_bar->text().setCharacterSize(stats_font_size);
    progress_bar->text().setOutlineColor(sf::Color::Black);
    progress_bar->text().setOutlineThickness(1.0f);
    progress_bar->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    progress_bar->text().setPosition(stats_progress_bar_text_offset);

    return progress_bar;
}

ProgressBar *Style::createTargetHpBar()
{
    ProgressBar *progress_bar = new ProgressBar{
        *ResourceCache::getResource<sf::Texture>(ui_texture_name),
        frame_texture_rect,
        progress_bar_red_texture_rect,
        progress_bar_offset};

    progress_bar->setBorderSize(frame_border_size);
    progress_bar->setFrameSize(hp_progress_bar_size);
    progress_bar->setScale(vector2::mult(stats_scale, {-1.0f, 1.0f}));

    progress_bar->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    progress_bar->text().setCharacterSize(stats_font_size);
    progress_bar->text().setOutlineColor(sf::Color::Black);
    progress_bar->text().setOutlineThickness(1.0f);
    progress_bar->text().setScale(vector2::div({-1.0f, 1.0f}, stats_scale));
    progress_bar->text().setPosition(stats_progress_bar_text_offset);

    return progress_bar;
}

Minimap *Style::createMinimap()
{
    Minimap *minimap = new Minimap{
        *ResourceCache::getResource<sf::Texture>(ui_texture_name),
        frame_transparented_texture_rect,
        minimap_room_texture_rects,
        minimap_room_type_texture_rects,
        minimap_current_room_texture_rect};

    minimap->setBorderSize(frame_border_size);
    minimap->setFrameSize(minimap_size);
    minimap->setScale(stats_scale);

    minimap->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    minimap->text().setCharacterSize(stats_font_size);
    minimap->text().setOutlineColor(sf::Color::Black);
    minimap->text().setOutlineThickness(1.0f);
    minimap->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    minimap->text().setPosition(
        {minimap_text_offset.x, minimap->getLocalBounds().height + minimap_text_offset.y});
    minimap->text().setString("M");

    return minimap;
}

Button *Style::createMainMenuButton(const sf::String &text)
{
    Button *button = new Button{
        *ResourceCache::getResource<sf::Texture>(ui_texture_name),
        main_menu_button_default_color,
        main_menu_button_default_frame_texture_rect,
        main_menu_button_hover_color,
        main_menu_button_hover_frame_texture_rect,
        main_menu_button_pressed_color,
        main_menu_button_pressed_frame_texture_rect};

    button->setBorderSize(frame_border_size);
    button->setScale(stats_scale);

    button->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    button->text().setCharacterSize(main_menu_button_font_size);
    button->text().setOutlineColor(sf::Color::Black);
    button->text().setOutlineThickness(1.0f);
    button->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    button->text().setPosition({4.0f, 0.5f});
    button->text().setString(text);

    button->setFrameSize(button->text().getGlobalBounds().getSize() + sf::Vector2f{8.0f, 8.0f});

    return button;
}

QuestionDialog *Style::createQuestionDialog(
    const sf::String &text, const sf::String &ok_text, const sf::String &cancel_text)
{
    QuestionDialog *question_dialog = new QuestionDialog{
        text,
        ok_text,
        cancel_text,
        *ResourceCache::getResource<sf::Texture>(ui_texture_name),
        dialog_frame_texture_rect};

    question_dialog->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    question_dialog->text().setCharacterSize(main_menu_button_font_size);
    question_dialog->text().setOutlineColor(sf::Color::Black);
    question_dialog->text().setOutlineThickness(1.0f);
    question_dialog->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    question_dialog->text().setPosition({4.0f, 0.5f});
    question_dialog->text().setString(text);

    question_dialog->setBorderSize(frame_border_size);
    question_dialog->setScale(stats_scale);

    Button *ok_button = question_dialog->okButton();
    createQuestionDialogButton(ok_button, ok_text);

    Button *cancel_button = question_dialog->cancelButton();
    createQuestionDialogButton(cancel_button, cancel_text);

    float buttons_height
        = std::max(ok_button->getFrameSize().y * 2, cancel_button->getFrameSize().y * 2);

    question_dialog->setFrameSize(
        question_dialog->text().getGlobalBounds().getSize()
        + sf::Vector2f{8.0f, buttons_height + 0.0f});

    cancel_button->setPosition(
        question_dialog->getFrameSize() - sf::Vector2f{4.0f, 4.0f} - cancel_button->getFrameSize());

    ok_button->setPosition(
        cancel_button->getPosition() - sf::Vector2f{4.0f + ok_button->getFrameSize().x, 0.0f});

    return question_dialog;
}

void Style::createQuestionDialogButton(Button *button, const sf::String &text)
{
    button->setTexture(*ResourceCache::getResource<sf::Texture>(ui_texture_name));
    button->setDefaultColor(main_menu_button_default_color);
    button->setDefaultFrameTextureRect(main_menu_button_default_frame_texture_rect);
    button->setHoveredColor(main_menu_button_hover_color);
    button->setHoveredFrameTextureRect(main_menu_button_hover_frame_texture_rect);
    button->setPressedColor(main_menu_button_pressed_color);
    button->setPressedFrameTextureRect(main_menu_button_pressed_frame_texture_rect);

    button->setBorderSize(frame_border_size);
    button->setScale({1.0f, 1.0f});

    button->text().setFont(*ResourceCache::getResource<sf::Font>(ui_font_name));
    button->text().setCharacterSize(main_menu_button_font_size);
    button->text().setOutlineColor(sf::Color::Black);
    button->text().setOutlineThickness(1.0f);
    button->text().setScale(vector2::div({1.0f, 1.0f}, stats_scale));
    button->text().setPosition({4.0f, 0.5f});
    button->text().setString(text);

    button->setFrameSize(button->text().getGlobalBounds().getSize() + sf::Vector2f{8.0f, 8.0f});
}

} // namespace fck::gui
