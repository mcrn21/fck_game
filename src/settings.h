#ifndef SETTINGS_YSZGPDCGTBNI_H
#define SETTINGS_YSZGPDCGTBNI_H

#include "fck/base_settings.h"

namespace fck
{

class Settings : public BaseSettings<Settings>
{
public:
    Settings();
    ~Settings() = default;

    bool loadFromFile(const std::string &file_name);

public:
    uint32_t window_width;
    uint32_t window_height;
    bool fullscreen;

    std::string level_keyboard_actions_file_name;
    std::string main_menu_keyboard_actions_file_name;

    std::string splash_screen_bg_file_name;
    std::string ui_file_name;
    std::string font_file_name;

    std::string resources_database_name;

    std::string fonts_dir_name;
    std::string textures_dir_name;
    std::string sounds_dir_name;
    std::string sprites_dir_name;
    std::string skills_dir_name;
    std::string entities_dir_name;
    std::string levels_dir_name;
    std::string scripts_dir_name;
};

} // namespace fck

#endif // SETTINGS_YSZGPDCGTBNI_H
