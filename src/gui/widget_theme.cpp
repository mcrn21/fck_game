#include "widget_theme.h"
#include "button.h"
#include "dialog.h"
#include "minimap.h"
#include "progress_bar.h"

#include "../fck/resource_cache.h"

namespace fck::gui
{

std::unordered_map<std::type_index, WidgetTheme> WidgetTheme::m_themes = []() {
    std::unordered_map<std::type_index, WidgetTheme> themes;

    // base theme
    WidgetTheme default_theme;

    default_theme.texture = "ui";
    default_theme.font = "font";
    default_theme.padding = 16.0f;
    default_theme.direction = Direction::RIGHT_TO_LEFT;

    // background
    default_theme.background.border_size = 16.0f;

    default_theme.background.texture_rects[WidgetState::DEFAULT] = {{0, 0}, {32, 32}};
    default_theme.background.texture_rects[WidgetState::HOVERED] = {{32, 0}, {32, 32}};
    default_theme.background.texture_rects[WidgetState::PRESSED] = {{64, 0}, {32, 32}};
    default_theme.background.texture_rects[WidgetState::FOCUSED] = {{96, 0}, {32, 32}};

    default_theme.background.texture_border_size = 8;

    // foreground
    default_theme.foreground.border_size = 0.0f;

    default_theme.foreground.texture_rects[WidgetState::DEFAULT] = {{0, 160}, {32, 32}};
    default_theme.foreground.texture_rects[WidgetState::HOVERED] = {{0, 160}, {32, 32}};
    default_theme.foreground.texture_rects[WidgetState::PRESSED] = {{0, 160}, {32, 32}};
    default_theme.foreground.texture_rects[WidgetState::FOCUSED] = {{0, 160}, {32, 32}};

    default_theme.foreground.texture_border_size = 0;

    // text
    default_theme.text.character_size = 34;
    default_theme.text.style = sf::Text::Regular;

    default_theme.text.fill_colors[WidgetState::DEFAULT] = sf::Color::White;
    default_theme.text.fill_colors[WidgetState::HOVERED] = sf::Color::White;
    default_theme.text.fill_colors[WidgetState::PRESSED] = sf::Color::White;
    default_theme.text.fill_colors[WidgetState::FOCUSED] = sf::Color::White;

    default_theme.text.outline_thickness = 0.0f;

    default_theme.text.outline_colors[WidgetState::DEFAULT] = sf::Color::Black;
    default_theme.text.outline_colors[WidgetState::HOVERED] = sf::Color::Black;
    default_theme.text.outline_colors[WidgetState::PRESSED] = sf::Color::Black;
    default_theme.text.outline_colors[WidgetState::FOCUSED] = sf::Color::Black;

    default_theme.text.align = TextAlign::CENTER;

    // header
    default_theme.header.padding = {24.0f, 14.0f, 24.0f, 14.0f};

    // close button
    default_theme.close.border_size = 0.0f;

    default_theme.close.texture_rects[WidgetState::DEFAULT] = {{0, 128}, {32, 32}};
    default_theme.close.texture_rects[WidgetState::HOVERED] = {{32, 128}, {32, 32}};
    default_theme.close.texture_rects[WidgetState::PRESSED] = {{64, 128}, {32, 32}};
    default_theme.close.texture_rects[WidgetState::FOCUSED] = {{32, 128}, {32, 32}};

    default_theme.close.texture_border_size = 0;

    themes.emplace(typeid(Widget), default_theme);

    // button
    WidgetTheme button_theme = default_theme;

    button_theme.background.texture_rects[WidgetState::DEFAULT] = {{0, 64}, {32, 32}};
    button_theme.background.texture_rects[WidgetState::HOVERED] = {{32, 64}, {32, 32}};
    button_theme.background.texture_rects[WidgetState::PRESSED] = {{64, 64}, {32, 32}};
    button_theme.background.texture_rects[WidgetState::FOCUSED] = {{96, 64}, {32, 32}};

    button_theme.text.fill_colors[WidgetState::DEFAULT] = sf::Color::White;
    button_theme.text.fill_colors[WidgetState::HOVERED] = sf::Color::Black;
    button_theme.text.fill_colors[WidgetState::PRESSED] = sf::Color::Black;
    button_theme.text.fill_colors[WidgetState::FOCUSED] = sf::Color::White;

    themes.emplace(typeid(Button), button_theme);

    // dialog
    WidgetTheme dialog_theme = default_theme;

    dialog_theme.padding = {24.0f, 48.0f, 24.f, 24.0f};

    dialog_theme.background.border_size = 24.0f;

    dialog_theme.background.texture_rects[WidgetState::DEFAULT] = {{0, 96}, {32, 32}};
    dialog_theme.background.texture_rects[WidgetState::HOVERED] = {{0, 96}, {32, 32}};
    dialog_theme.background.texture_rects[WidgetState::PRESSED] = {{0, 96}, {32, 32}};
    dialog_theme.background.texture_rects[WidgetState::FOCUSED] = {{0, 96}, {32, 32}};

    dialog_theme.background.texture_border_size = {8, 8, 8, 8};

    themes.emplace(typeid(Dialog), dialog_theme);

    // Progress bar
    WidgetTheme progress_bar_theme = default_theme;

    progress_bar_theme.padding = {8.0f, 8.0f, 8.0f, 12.0f};

    themes.emplace(typeid(ProgressBar), progress_bar_theme);

    // Minimap
    WidgetTheme minimap_bar_theme = default_theme;

    minimap_bar_theme.padding = 10.0f;

    minimap_bar_theme.background.texture_rects[WidgetState::DEFAULT] = {{0, 32}, {32, 32}};
    minimap_bar_theme.background.texture_rects[WidgetState::HOVERED] = {{32, 32}, {32, 32}};
    minimap_bar_theme.background.texture_rects[WidgetState::PRESSED] = {{64, 32}, {32, 32}};
    minimap_bar_theme.background.texture_rects[WidgetState::FOCUSED] = {{96, 32}, {32, 32}};

    themes.emplace(typeid(Minimap), minimap_bar_theme);

    return themes;
}();

std::type_index WidgetTheme::getDefaultTypeIndex()
{
    return typeid(Widget);
}

} // namespace fck::gui
