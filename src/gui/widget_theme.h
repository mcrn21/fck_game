#ifndef WIDGETTHEME_H
#define WIDGETTHEME_H

#include "common.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <typeindex>
#include <unordered_map>

namespace fck::gui
{

struct WidgetTheme
{
    std::string texture;
    std::string font;

    Sides<float> padding;

    Direction direction;

    struct Background
    {
        Sides<float> border_size;
        std::unordered_map<WidgetState, sf::IntRect> texture_rects;
        Sides<int32_t> texture_border_size;
    } background;

    struct Foreground
    {
        Sides<float> border_size;
        std::unordered_map<WidgetState, sf::IntRect> texture_rects;
        Sides<int32_t> texture_border_size;
    } foreground;

    struct Text
    {
        sf::Font *font;
        int32_t character_size;
        sf::Text::Style style;
        std::unordered_map<WidgetState, sf::Color> fill_colors;
        float outline_thickness;
        std::unordered_map<WidgetState, sf::Color> outline_colors;
        int32_t align;
    } text;

    struct Header
    {
        Sides<float> padding;
    } header;

    struct Close
    {
        Sides<float> border_size;
        std::unordered_map<WidgetState, sf::IntRect> texture_rects;
        Sides<int32_t> texture_border_size;
    } close;

    template<typename T>
    static const WidgetTheme &get()
    {
        auto themes_found = m_themes.find(typeid(T));
        if (themes_found == m_themes.end())
            return m_themes.at(getDefaultTypeIndex());
        return themes_found->second;
    }

private:
    static std::type_index getDefaultTypeIndex();

private:
    static std::unordered_map<std::type_index, WidgetTheme> m_themes;
};

} // namespace fck::gui

#endif // WIDGETTHEME_H
