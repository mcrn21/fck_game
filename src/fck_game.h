#ifndef FCKGAME_LHJLOJYRDNWT_H
#define FCKGAME_LHJLOJYRDNWT_H

#include "fck/b2_dynamic_tree.h"
#include "fck/base_game.h"
#include "fck/event_handler.h"
#include "fck/input_actions_map.h"
#include "fck/world.h"
#include "fck_common.h"
#include "gui/gui.h"
#include "map/map.h"
#include "systems/systems.h"

#include <SFML/Graphics.hpp>

#include <sol/sol.hpp>

#include <list>
#include <memory>

namespace fck
{

class FckGame : public BaseGame
{
public:
    FckGame();
    ~FckGame() = default;

    void init();

protected:
    void event(const sf::Event &e);
    void update(const sf::Time &elapsed);
    void draw(const sf::Time &elapsed);

private:
    void setState(game_state::State state);

    void event(Event *event);

    void initFirstResources();
    void loadFonts();
    void loadTextures();
    void loadSounds();

    void exitGame();
    void newGame();
    void returnToMainMenu();

    void setupInputActions();

public: // slots
    void onActionActivated(keyboard_action::Action action);

private:
    sigslot::signal<map::Map *> map_changed;

private:
    sf::RenderTexture m_scene_render_texture;
    sf::Sprite m_scene_render_sprite;
    sf::View m_render_window_view;

    std::unique_ptr<EventHandler> m_event_handler;

    game_state::State m_state;

    gui::MainWidget m_main_widget;

    InputActionsMap<keyboard_action::Action> m_input_actions;

    sf::View m_scene_view;
    std::list<Entity> m_visible_entities;

    World m_world;

    b2::DynamicTree<Entity> m_render_tree;
    b2::DynamicTree<Entity> m_scene_tree;

    std::unique_ptr<map::Map> m_map;
    Entity m_player_entity;

    system::Render m_render_system;
    system::Player m_player_system;
    system::Movement m_movement_system;
    system::ViewMovement m_view_movement_system;
    system::DrawableAnimation m_drawable_animation_system;
    system::Collision m_collision_system;
    system::Scene m_scene_system;
    system::Script m_script_system;
    system::TargetFollow m_target_follow_system;
    system::LookAround m_look_around_system;
    system::Stats m_stats_system;
    system::Skills m_skills_system;
    system::Damage m_damage_sysytem;
    system::Sound m_sound_system;

    bool m_render_debug;

    sol::state m_lua_state;
};

} // namespace fck

#endif // FCKGAME_LHJLOJYRDNWT_H
