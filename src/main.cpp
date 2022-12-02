#include "fck_game.h"
#include "settings.h"

#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Welcome to fck_game!");

    auto settings = std::make_shared<fck::Settings>();
    settings->loadFromFile("settings.toml");
    fck::Settings::setGlobal(settings);

    fck::FckGame game;
    game.init();

    return game.exec();
}
