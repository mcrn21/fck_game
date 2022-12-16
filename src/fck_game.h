#ifndef FCKGAME_LHJLOJYRDNWT_H
#define FCKGAME_LHJLOJYRDNWT_H

#include "fck_common.h"
#include "gui_manager.h"
#include "level.h"

#include "systems/systems.h"

#include "fck/b2_dynamic_tree.h"
#include "fck/base_game.h"
#include "fck/input_actions_map.h"
#include "fck/world.h"

#include <SFML/Graphics.hpp>

#include <list>
#include <memory>

namespace fck
{

class FckGame : public BaseGame
{
public:
    static FckGame *getInstance();

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

    void exitGame();
    void newGame();
    void returnToMainMenu();

    void setupInputActions();

public: // slots
    void onActionActivated(keyboard_action::Action action);

    // world
    void onWorldEntityEnabled(const Entity &entity);
    void onWorldEntityDisabled(const Entity &entity);
    void onWorldEntityDestroyed(const Entity &entity);

    // transform
    void entityMove(const Entity &entity, const sf::Vector2f &offset);
    void entitySetPosition(const Entity &entity, const sf::Vector2f &position);
    void entitySetParent(const Entity &entity, const Entity &parent);

    // state
    void entitySetState(const Entity &entity, entity_state::State state);
    void entitySetDirection(const Entity &entity, entity_state::Direction direction);

    // taregt
    void entitySetTarget(const Entity &entity, const Entity &target);

    // marker
    void entitySetMarker(const Entity &entity, const Entity &marker);

    // stats
    void entitySetHealth(const Entity &entity, float health);
    void entitySetArmor(const Entity &entity, float armor);

    // destroy
    void entityDestroy(const Entity &entity);

    // collided
    void entityCollided(const Entity &entity, const Entity &other);

    // drawable
    void entityDrawableSetState(const Entity &entity, const std::string &state);

    // sound
    void entityPlaySound(const Entity &entity, const std::string &sound);
    void entityStopSound(const Entity &entity);

    // other
    void entitySetTileMaterial(const Entity &entity, tile_material_type::Type tile_material);

    // level
    void onLevelRoomOpened(const sf::Vector2i &room_coord);
    void onLevelRoomEnabled(const sf::Vector2i &room_coord);

private:
    sf::RenderTexture m_scene_render_texture;
    sf::Sprite m_scene_render_sprite;
    sf::View m_render_window_view;

    std::unique_ptr<EventHandler> m_event_handler;

    game_state::State m_state;

    GuiManager m_gui_manager;

    InputActionsMap<keyboard_action::Action> m_input_actions;

    sf::View m_scene_view;
    std::list<Entity> m_visible_entities;

    World m_world;

    b2::DynamicTree<Entity> m_render_tree;
    b2::DynamicTree<Entity> m_scene_tree;

    std::unique_ptr<Level> m_level;
    Entity m_player_entity;

    system::Render m_render_system;
    system::PlayerActions m_player_actions_system;
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

    bool m_render_debug;
};

} // namespace fck

#endif // FCKGAME_LHJLOJYRDNWT_H
