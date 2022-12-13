#include "settings.h"

namespace fck
{

Settings::Settings()
{
    window_width = 1024;
    window_height = 768;
    fullscreen = false;

    level_keyboard_actions_file_name = "l_ka.toml";
    main_menu_keyboard_actions_file_name = "mm_ka.toml";
    splash_screen_bg_file_name = "resources/textures/splash.png";
    ui_file_name = "resources/textures/ui.png";
    font_file_name = "resources/fonts/mini_pixel-7.ttf";

    resources_database_name = "resources/game_data";

    fonts_dir_name = "resources/fonts";
    textures_dir_name = "resources/textures";
    sounds_dir_name = "resources/sounds";
    sprites_dir_name = "resources/sprites";
    skills_dir_name = "resources/skills";
    entities_dir_name = "resources/entities";
    levels_dir_name = "resources/levels";
    scripts_dir_name = "resources/scripts";
}

bool Settings::loadFromFile(const std::string &file_name)
{
    return true;
}

} // namespace fck
