#ifndef GUISTYLE_FQQFPDQRPGZG_H
#define GUISTYLE_FQQFPDQRPGZG_H

#include "../level.h"

#include <SFML/Graphics.hpp>

#include <any>
#include <string>

namespace fck::gui
{

class ProgressBar;
class Minimap;
class Button;
class QuestionDialog;

struct Style
{
    static std::string ui_texture_name;
    static std::string ui_font_name;

    static sf::Vector2f viewport_offset;
    static sf::Vector2f stats_scale;

    static sf::IntRect frame_texture_rect;
    static sf::IntRect frame_transparented_texture_rect;
    static sf::Vector2i frame_border_size;

    static sf::IntRect progress_bar_red_texture_rect;
    static sf::IntRect progress_bar_blue_texture_rect;
    static sf::IntRect progress_bar_yellow_texture_rect;
    static sf::Vector2f progress_bar_offset;

    static sf::Vector2f loading_progress_bar_size;
    static sf::Vector2f loading_progress_bar_text_offset;
    static uint32_t loading_progress_bar_font_size;

    static sf::Color main_menu_button_default_color;
    static sf::IntRect main_menu_button_default_frame_texture_rect;
    static sf::Color main_menu_button_hover_color;
    static sf::IntRect main_menu_button_hover_frame_texture_rect;
    static sf::Color main_menu_button_pressed_color;
    static sf::IntRect main_menu_button_pressed_frame_texture_rect;
    static uint32_t main_menu_button_font_size;

    static sf::IntRect dialog_frame_texture_rect;

    static sf::Vector2f hp_progress_bar_size;
    static sf::Vector2f armor_progress_bar_size;
    static sf::Vector2f stats_progress_bar_text_offset;
    static uint32_t stats_font_size;

    static std::unordered_map<int32_t, sf::IntRect> minimap_room_texture_rects;
    static std::unordered_map<room_type::Type, sf::IntRect> minimap_room_type_texture_rects;
    static sf::IntRect minimap_current_room_texture_rect;
    static sf::Vector2f minimap_size;
    static sf::Vector2f minimap_text_offset;

    static ProgressBar *createLoadingProgressBar();

    static ProgressBar *createPlayerHpBar();
    static ProgressBar *createPlayerArmorBar();
    static ProgressBar *createTargetHpBar();

    static Minimap *createMinimap();

    static Button *createMainMenuButton(const sf::String &text);

    static QuestionDialog *createQuestionDialog(
        const sf::String &text, const sf::String &ok_text, const sf::String &cancel_text);
    static void createQuestionDialogButton(Button *button, const sf::String &text);

    static Style frame_style;

    std::unordered_map<std::string, std::any> values;
};

} // namespace fck::gui

#endif // GUISTYLE_FQQFPDQRPGZG_H
