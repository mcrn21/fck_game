#ifndef FCKGAME_H
#define FCKGAME_H

#include "fck_common.h"
#include "gui_base.h"

#include "systems/systems.h"

#include "fck/a_star.h"
#include "fck/b2_dynamic_tree.h"
#include "fck/base_game.h"
#include "fck/input_actions_map.h"
#include "fck/world.h"

#include <SFML/Graphics.hpp>

#include <sol/sol.hpp>

#include <list>
#include <memory>

namespace fck
{

class FckGame : public BaseGame
{
public:
    static FckGame *instance();

    FckGame();
    ~FckGame() = default;

    void init();

protected:
    void update(const sf::Time &elapsed);
    void draw(const sf::Time &elapsed);

private:
    void setState(game_state::State state);

    void event(Event *event);

    void initFirstResources();
    void loadFonts();
    void loadTextures();
    void loadSounds();
    void loadScripts();

    void exitGame();
    void newGame();
    void returnToMainMenu();

    void setupInputActions();

public: // slots
    void onActionActivated(keyboard_action::Action action);

    void onEntityMoved(const Entity &entity, const sf::Vector2f &offset);

    void onEntityStateChanged(const Entity &entity, entity_state::State old_state);
    void onEntityDirectionChanged(const Entity &entity, entity_state::Direction old_direction);

    void onEntityParentChanged(const Entity &entity, const Entity &old_parent);

private:
    sf::RenderTexture m_scene_render_texture;
    sf::Sprite m_scene_render_sprite;
    sf::View m_render_window_view;

    std::unique_ptr<EventHandler> m_event_handler;

    game_state::State m_state;

    std::list<std::unique_ptr<GuiBase>> m_gui_list;

    InputActionsMap<keyboard_action::Action> m_input_actions;

    sf::View m_scene_view;
    std::list<Entity> m_visible_entities;

    World m_world;

    b2::DynamicTree<Entity> m_render_tree;
    b2::DynamicTree<Entity> m_scene_tree;

    PathFinder m_path_finder;

    Entity m_player_entity;

    RenderSystem m_render_system;
    PlayerActionsSystem m_player_actions_system;
    MovementSystem m_movement_system;
    ViewMovementSystem m_view_movement_system;
    DrawableAnimationSystem m_drawable_animation_system;
    CollisionSystem m_collision_system;
    SceneSystem m_scene_system;
    ScriptSystem m_script_system;
    TargetFollowSystem m_target_follow_system;
    LookAroundSystem m_look_around_system;
    ActionsSystem m_actions_system;
    StatsSystem m_stats_system;
    SkillsSystem m_skills_system;
    DamageSystem m_damage_sysytem;

    bool m_render_debug;

    sol::state m_lua;
};

} // namespace fck

#endif // FCKGAME_H
