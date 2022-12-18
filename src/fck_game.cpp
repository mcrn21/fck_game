#include "fck_game.h"
#include "debug_draw.h"
#include "entity_utils.h"
#include "events.h"
#include "knowledge_base.h"
#include "settings.h"

#include "components/components.h"
#include "damages/damages.h"
#include "entity_scripts/entity_scripts.h"
#include "gui/gui.h"
#include "skills/skills.h"

#include "fck/clipping.h"
#include "fck/event_dispatcher.h"
#include "fck/resource_cache.h"
#include "fck/sprite_animation.h"
#include "fck/task_sequence.h"
#include "fck/tile_map.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include <spdlog/spdlog.h>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <algorithm>
#include <filesystem>

namespace fck
{

FckGame *FckGame::getInstance()
{
    return static_cast<FckGame *>(BaseGame::getInstance());
}

FckGame::FckGame()
    : BaseGame{},
      m_render_window_view{sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1, 1))},
      m_scene_view{sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1, 1))},
      m_render_system{&m_render_tree},
      m_scene_system{&m_scene_tree},
      m_look_around_system{&m_scene_tree},
      m_render_debug{false}
{
    m_event_handler = std::make_unique<EventHandler>(
        std::vector<int32_t>{
            sf::Event::Resized,
            sf::Event::Closed,
            event_type::EXIT_GAME,
            event_type::NEW_GAME,
            event_type::RETURN_TO_GAME,
            event_type::RETURN_TO_MAIN_MENU},
        std::bind(&FckGame::event, this, std::placeholders::_1));

    getRrenderWindow().setView(m_render_window_view);

    m_world.addSystem(m_render_system);
    m_world.addSystem(m_player_actions_system);
    m_world.addSystem(m_movement_system);

    m_world.addSystem(m_view_movement_system);
    m_view_movement_system.setView(&m_scene_view);

    m_world.addSystem(m_drawable_animation_system);
    m_world.addSystem(m_collision_system);
    m_world.addSystem(m_scene_system);
    m_world.addSystem(m_script_system);
    m_world.addSystem(m_target_follow_system);
    m_world.addSystem(m_look_around_system);
    m_world.addSystem(m_stats_system);
    m_world.addSystem(m_skills_system);
    m_world.addSystem(m_damage_sysytem);

    // world
    m_world.entity_enabled.connect(this, &FckGame::onWorldEntityEnabled);
    m_world.entity_disabled.connect(this, &FckGame::onWorldEntityDisabled);
    m_world.entity_destroyed.connect(this, &FckGame::onWorldEntityDestroyed);

    // transform
    entity::move.connect(this, &FckGame::entityMove);
    entity::set_position.connect(this, &FckGame::entitySetPosition);
    entity::set_parent.connect(this, &FckGame::entitySetParent);

    // state
    entity::set_state.connect(this, &FckGame::entitySetState);
    entity::set_direction.connect(this, &FckGame::entitySetDirection);

    // taregt
    entity::set_target.connect(this, &FckGame::entitySetTarget);

    // marker
    entity::set_marker.connect(this, &FckGame::entitySetMarker);

    // stats
    entity::set_heath.connect(this, &FckGame::entitySetHealth);
    entity::set_armor.connect(this, &FckGame::entitySetArmor);

    // destroy
    entity::destroy.connect(this, &FckGame::entityDestroy);

    // collided
    entity::collided.connect(this, &FckGame::entityCollided);

    // drawable
    entity::set_drawable_state.connect(this, &FckGame::entityDrawableSetState);

    // drawable
    entity::play_sound.connect(this, &FckGame::entityPlaySound);
    entity::stop_sound.connect(this, &FckGame::entityStopSound);
    entity::stop_all_sound.connect(this, &FckGame::entityStopAllSound);

    // other
    entity::set_tile_material.connect(this, &FckGame::entitySetTileMaterial);

    // Global value
    sf::Listener::setGlobalVolume(100);
}

void FckGame::init()
{
    auto settings = Settings::getGlobal();

    initFirstResources();

    sf::ContextSettings contex_settings;
    contex_settings.antialiasingLevel = 4;
    getRrenderWindow().create(
        sf::VideoMode({settings->window_width, settings->window_height}),
        std::string{"fck_game"},
        settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Default,
        contex_settings);

    TaskSequence *first_loading_tasks = new TaskSequence();
    first_loading_tasks->setTasks(
        {[this, first_loading_tasks]() {
             setState(game_state::FIRST_LOADING);
             //             first_loading_tasks->pause();
             //             Timer::singleShot(
             //                 sf::milliseconds(1000), [first_loading_tasks]() { first_loading_tasks->start(); });
         },
         [this, first_loading_tasks]() {
             setState(game_state::LOADING);
             gui::LoadingGui *loading_gui = m_gui_manager.getBack<gui::LoadingGui>();
             loading_gui->setTotal(first_loading_tasks->getTasks().size() - 2);
             first_loading_tasks->task_finished.connect(
                 loading_gui, &gui::LoadingGui::increaseProgress);
             loadFonts();
         },
         [this]() { loadTextures(); },
         [this]() { loadSounds(); },
         [this]() {
             KnowledgeBase::loadSkillsFromDatabase(Settings::getGlobal()->resources_database_name);
         },
         [this]() {
             KnowledgeBase::loadEntityScriptsFromDatabase(
                 Settings::getGlobal()->resources_database_name);
         },
         [this]() {
             KnowledgeBase::loadEntitiesFromDatabase(
                 Settings::getGlobal()->resources_database_name);
         },
         [this]() { setupInputActions(); },
         [this, first_loading_tasks]() {
             first_loading_tasks->task_finished.disconnect();
             setState(game_state::MAIN_MENU);
             first_loading_tasks->deleteLater();
         }});

    first_loading_tasks->start();
}

void FckGame::update(const sf::Time &elapsed)
{
    m_world.refresh();

    if (m_state == game_state::LEVEL)
    {
        double delta_time = double(elapsed.asMilliseconds()) / 1000;

        m_visible_entities.clear();

        m_player_actions_system.update(delta_time);
        m_target_follow_system.update(delta_time);

        m_look_around_system.update(delta_time);

        m_script_system.update(delta_time);

        m_skills_system.update(delta_time);
        m_damage_sysytem.update(delta_time);
        m_stats_system.update(delta_time);

        m_collision_system.update(delta_time);
        m_movement_system.update(delta_time);

        m_view_movement_system.update(delta_time);
        m_drawable_animation_system.update(elapsed);

        // Update visible entities
        sf::Vector2f view_pos = m_scene_view.getCenter();
        sf::Vector2f view_size = m_scene_view.getSize();
        sf::FloatRect viewport_rect(
            sf::Vector2f(view_pos.x - view_size.x / 2, view_pos.y - view_size.y / 2),
            sf::Vector2f(view_size.x, view_size.y));

        m_render_tree.querry(viewport_rect, [this](int32_t node_id) {
            Entity entity = m_render_tree.getUserData(node_id);
            m_visible_entities.push_back(entity);
            return true;
        });

        m_visible_entities.sort([](const Entity &a, const Entity &b) {
            component::Drawable &dca = a.getComponent<component::Drawable>();
            component::Drawable &dcb = b.getComponent<component::Drawable>();
            return dca.z_order < dcb.z_order;
        });
    }
}

void FckGame::draw(const sf::Time &elapsed)
{
    // Draw scene
    m_scene_render_texture.clear();
    m_scene_render_texture.setView(m_scene_view);

    if (m_state == game_state::LEVEL || m_state == game_state::LEVEL_MENU)
    {
        component::Drawable *player_drawable_component = nullptr;
        std::optional<Entity> player_target_entity;
        if (m_player_entity.isValid())
        {
            player_drawable_component = &m_player_entity.getComponent<component::Drawable>();
            player_target_entity = m_player_entity.getComponent<component::Target>().target;
        }

        for (Entity &entity : m_visible_entities)
        {
            if (entity.hasComponent<component::Drawable>())
            {
                component::Drawable &drawable_component
                    = entity.getComponent<component::Drawable>();
                component::Transform &transform_component
                    = entity.getComponent<component::Transform>();
                bool transparented = false;

                if (player_drawable_component)
                {
                    auto intersects = drawable_component.global_bounds.findIntersection(
                        player_drawable_component->global_bounds);
                    if (intersects.has_value()
                        && drawable_component.z_order > player_drawable_component->z_order)
                    {
                        if (intersects->width
                                > player_drawable_component->global_bounds.width * 0.9f
                            && intersects->height
                                > player_drawable_component->global_bounds.height * 0.9f)
                            transparented = true;
                    }
                }

                if (transparented)
                    drawable_component.proxy->setColor(sf::Color(255, 255, 255, 150));

                sf::RenderStates render_states;
                render_states.transform *= transform_component.transform.getTransform();

                if (entity.hasComponent<component::Shadow>())
                {
                    component::Shadow &shadow_component = entity.getComponent<component::Shadow>();
                    if (shadow_component.shadow_shape)
                        m_scene_render_texture.draw(*shadow_component.shadow_shape, render_states);
                }

                m_scene_render_texture.draw(*drawable_component.proxy->toDrawable(), render_states);

                if (transparented)
                    drawable_component.proxy->setColor(sf::Color(255, 255, 255, 255));
            }
        }

        if (m_render_debug)
        {
            sf::Vector2f view_pos = m_scene_view.getCenter();
            sf::Vector2f view_size = m_scene_view.getSize();
            sf::FloatRect viewport_rect(
                sf::Vector2f(view_pos.x - view_size.x / 2, view_pos.y - view_size.y / 2),
                sf::Vector2f(view_size.x, view_size.y));

            m_scene_tree.querry(viewport_rect, [this](int32_t node_id) {
                Entity entity = m_scene_tree.getUserData(node_id);

                debug_draw::drawDrawableBounds(entity, m_scene_render_texture);
                //                debug_draw::drawSceneTreeAABB(entity, m_scene_render_texture);
                debug_draw::drawSceneBounds(entity, m_scene_render_texture);
                debug_draw::drawVelocity(entity, m_scene_render_texture); /*
                debug_draw::drawPathFinderCellsBounds(
                    entity, m_path_finder.getGrid().getCellSize(), m_scene_render_texture);*/
                if (m_level && m_level->getCurrentRoomCoord() != sf::Vector2i{-1, -1})
                {
                    debug_draw::drawTargetFollowPath(
                        entity,
                        m_level->getRoomsMap()
                            .getData(m_level->getCurrentRoomCoord())
                            ->getWalls()
                            .getTileSize(),
                        m_scene_render_texture);
                }
                debug_draw::drawLookAroundBound(entity, m_scene_render_texture);
                debug_draw::drawLookAroundLookBound(entity, m_scene_render_texture);

                debug_draw::drawEntityId(entity, m_scene_render_texture);

                return true;
            });
        }
    }

    m_scene_render_texture.display();

    getRrenderWindow().clear();

    getRrenderWindow().draw(m_scene_render_sprite);

    m_gui_manager.draw(getRrenderWindow(), sf::RenderStates{});

    getRrenderWindow().display();
}

void FckGame::setState(game_state::State state)
{
    if (m_state == state)
        return;

    game_state::State old_state = m_state;
    m_state = state;

    m_input_actions.action_activated.disconnect();

    switch (m_state)
    {
    case game_state::FIRST_LOADING: {
        m_gui_manager.push<gui::SplashscreenGui>();
        break;
    }
    case game_state::LOADING: {
        m_gui_manager.clear();
        m_gui_manager.push<gui::LoadingGui>();
        break;
    }
    case game_state::MAIN_MENU: {
        m_gui_manager.clear();
        gui::MainMenuGui *main_menu_gui = m_gui_manager.push<gui::MainMenuGui>();

        m_input_actions.action_activated.connect(
            main_menu_gui, &gui::MainMenuGui::onActionActivated);

        break;
    }
    case game_state::LEVEL: {
        m_gui_manager.pop();

        if (old_state != game_state::LEVEL_MENU)
            m_gui_manager.push<gui::LevelGui>(m_player_entity);

        m_input_actions.action_activated.connect(this, &FckGame::onActionActivated);
        m_input_actions.action_activated.connect(
            &m_player_actions_system, &system::PlayerActions::onActionActivated);
        m_input_actions.action_diactivated.connect(
            &m_player_actions_system, &system::PlayerActions::onActionDiactivated);

        break;
    }
    case game_state::LEVEL_MENU: {
        gui::MainMenuGui *main_menu_gui = m_gui_manager.push<gui::MainMenuGui>(true);

        m_input_actions.action_activated.connect(
            main_menu_gui, &gui::MainMenuGui::onActionActivated);

        break;
    }
    default:
        break;
    }
}

void FckGame::event(Event *event)
{
    if (event->type() == sf::Event::Resized)
    {
        SfmlEvent *e = static_cast<SfmlEvent *>(event);

        float scene_view_scale = 0.2;

        m_scene_view.setSize(sf::Vector2f(
            e->get().size.width * scene_view_scale + 0.5,
            e->get().size.height * scene_view_scale + 0.5));

        (void)(m_scene_render_texture.create(
            sf::Vector2u(e->get().size.width, e->get().size.height)));
        m_scene_render_texture.setSmooth(true);
        m_scene_render_sprite.setTexture(m_scene_render_texture.getTexture(), true);

        m_render_window_view.setSize(sf::Vector2f(e->get().size.width, e->get().size.height));
        m_render_window_view.setCenter(
            sf::Vector2f(e->get().size.width / 2, e->get().size.height / 2));

        getRrenderWindow().setView(m_render_window_view);

        Clipping::setOriginalView(m_render_window_view);
        m_gui_manager.setViewportSize(m_render_window_view.getSize());

        return;
    }

    if (event->type() == event_type::EXIT_GAME || event->type() == sf::Event::Closed)
    {
        exitGame();
        event->accept();
        return;
    }

    if (event->type() == event_type::NEW_GAME)
    {
        newGame();
        event->accept();
        return;
    }

    if (event->type() == event_type::RETURN_TO_GAME)
    {
        setState(game_state::LEVEL);
        event->accept();
        return;
    }

    if (event->type() == event_type::RETURN_TO_MAIN_MENU)
    {
        returnToMainMenu();
        event->accept();
        return;
    }
}

void FckGame::initFirstResources()
{
    auto settings = Settings::getGlobal();

    // Load splashscreen
    ResourceCache::loadFromFile<sf::Texture>("splash", settings->splash_screen_bg_file_name);
    ResourceCache::loadFromFile<sf::Texture>("ui", settings->ui_file_name);
    ResourceCache::loadFromFile<sf::Font>("mini_pixel-7", settings->font_file_name)
        ->setSmooth(true);
}

void FckGame::loadFonts()
{
    auto settings = Settings::getGlobal();

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(settings->fonts_dir_name))
    {
        if (entry.is_directory())
            continue;

        std::string file_name;
        if (entry.path().filename().has_stem())
            file_name = entry.path().filename().stem().string();
        else
            file_name = entry.path().filename().string();

        if (file_name.empty())
            continue;

        std::string file_path = entry.path().relative_path().string();

        sf::Font *font = ResourceCache::loadFromFile<sf::Font>(file_name, file_path);
        if (font)
            font->setSmooth(true);
    }
}

void FckGame::loadTextures()
{
    auto settings = Settings::getGlobal();

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(settings->textures_dir_name))
    {
        if (entry.is_directory())
            continue;

        std::string file_name;
        if (entry.path().filename().has_stem())
            file_name = entry.path().filename().stem().string();
        else
            file_name = entry.path().filename().string();

        if (file_name.empty())
            continue;

        std::string file_path = entry.path().relative_path().string();

        ResourceCache::loadFromFile<sf::Texture>(file_name, file_path);
    }
}

void FckGame::loadSounds()
{
    auto settings = Settings::getGlobal();

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(settings->sounds_dir_name))
    {
        if (entry.is_directory())
            continue;

        std::string file_name;
        if (entry.path().filename().has_stem())
            file_name = entry.path().filename().stem().string();
        else
            file_name = entry.path().filename().string();

        if (file_name.empty())
            continue;

        std::string file_path = entry.path().relative_path().string();

        ResourceCache::loadFromFile<sf::SoundBuffer>(file_name, file_path);
    }
}

void FckGame::exitGame()
{
    exit();
}

void FckGame::newGame()
{
    TaskSequence *loading_new_game_tasks = new TaskSequence();
    loading_new_game_tasks->setTasks(
        {[this]() { setState(game_state::LOADING); },
         [this, loading_new_game_tasks]() {
             gui::LoadingGui *loading_gui = m_gui_manager.getBack<gui::LoadingGui>();
             loading_gui->setTotal(loading_new_game_tasks->getTasks().size() - 2);
             loading_new_game_tasks->task_finished.connect(
                 loading_gui, &gui::LoadingGui::increaseProgress);

             m_level = std::make_unique<Level>(&m_world, &m_scene_tree);
             m_level->room_opened.connect(this, &FckGame::onLevelRoomOpened);
             m_level->room_enabled.connect(this, &FckGame::onLevelRoomEnabled);
             m_level->loadFromFile("resources/levels/level2.tmx");
         },
         [this]() { m_level->generateRoomsMap(); },
         [this]() { m_level->generateRoomsContent(); },
         [this]() {
             m_view_movement_system.setMovementBounds({{0.0f, 0.0f}, m_level->getRoomPixelsSize()});

             m_player_entity = KnowledgeBase::createPlayer("kyoshi", &m_world);

             component::Script &player_entity_script_component
                 = m_player_entity.addComponent<component::Script>();
             player_entity_script_component.entity_script.reset(new entity_script::Player());

             entityMove(m_player_entity, sf::Vector2f{256.0f, 256.0f});

             Entity kyoshi_2 = KnowledgeBase::createEntity("kyoshi_2", &m_world);
             entityMove(kyoshi_2, {100.0f, 100.0f});
             entitySetState(kyoshi_2, entity_state::IDLE);
             entitySetDirection(kyoshi_2, entity_state::RIGHT);
             kyoshi_2.enable();

             m_player_entity.enable();
         },
         [this, loading_new_game_tasks]() {
             loading_new_game_tasks->task_finished.disconnect();

             setState(game_state::LEVEL);

             gui::LevelGui *level_gui = m_gui_manager.getBack<gui::LevelGui>();
             level_gui->setRoomsMap(m_level->getRoomsMap());

             m_level->enableRoom(m_level->getFirstRoomCoord(), {256.0f, 256.0f});

             loading_new_game_tasks->deleteLater();
         }});

    loading_new_game_tasks->start();
}

void FckGame::returnToMainMenu()
{
    TaskSequence *return_to_main_menu_tasks = new TaskSequence();
    return_to_main_menu_tasks->setTasks(
        {[this]() { setState(game_state::LOADING); },
         [this, return_to_main_menu_tasks]() {
             gui::LoadingGui *loading_gui = m_gui_manager.getBack<gui::LoadingGui>();
             loading_gui->setTotal(return_to_main_menu_tasks->getTasks().size() - 2);
             return_to_main_menu_tasks->task_finished.connect(
                 loading_gui, &gui::LoadingGui::increaseProgress);
             m_target_follow_system.setWalls(nullptr);
         },
         [this]() { m_level.reset(); },
         [this]() { m_world.destroyAllEntities(); },
         [this]() { m_visible_entities.clear(); },
         [this, return_to_main_menu_tasks]() {
             return_to_main_menu_tasks->task_finished.disconnect();

             setState(game_state::MAIN_MENU);

             return_to_main_menu_tasks->deleteLater();
         }});
    return_to_main_menu_tasks->start();
}

void FckGame::setupInputActions()
{
    m_input_actions[keyboard_action::BACK]
        = InputAction(sf::Keyboard::Escape, InputAction::RELEASE_ONCE);

    m_input_actions[keyboard_action::TOGGLE_RENDER_DEBUG]
        = InputAction(sf::Keyboard::F1, InputAction::RELEASE_ONCE);

    m_input_actions[keyboard_action::PLAYER_MOVE_LEFT]
        = InputAction(sf::Keyboard::A, InputAction::HOLD);
    m_input_actions[keyboard_action::PLAYER_MOVE_UP]
        = InputAction(sf::Keyboard::W, InputAction::HOLD);
    m_input_actions[keyboard_action::PLAYER_MOVE_RIGHT]
        = InputAction(sf::Keyboard::D, InputAction::HOLD);
    m_input_actions[keyboard_action::PLAYER_MOVE_DOWN]
        = InputAction(sf::Keyboard::S, InputAction::HOLD);

    m_input_actions[keyboard_action::CHANGE_TARGET]
        = InputAction(sf::Keyboard::Tab, InputAction::PRESS_ONCE);

    m_input_actions[keyboard_action::PLAYER_SKILL_1]
        = InputAction(sf::Keyboard::J, InputAction::PRESS_ONCE);
    m_input_actions[keyboard_action::PLAYER_SKILL_2]
        = InputAction(sf::Keyboard::K, InputAction::PRESS_ONCE);
    m_input_actions[keyboard_action::PLAYER_SKILL_3]
        = InputAction(sf::Keyboard::L, InputAction::PRESS_ONCE);
}

void FckGame::onActionActivated(keyboard_action::Action action)
{
    if (action == keyboard_action::TOGGLE_RENDER_DEBUG)
        m_render_debug = !m_render_debug;

    if (action == keyboard_action::BACK)
        EventDispatcher::runTask([this]() { setState(game_state::LEVEL_MENU); });
}

void FckGame::onWorldEntityEnabled(const Entity &entity)
{
    if (entity.hasComponent<component::Script>())
    {
        component::Script &script_component = entity.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityEnabled(entity);
    }
}

void FckGame::onWorldEntityDisabled(const Entity &entity)
{
    if (entity.hasComponent<component::Script>())
    {
        component::Script &script_component = entity.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityDisabled(entity);
    }

    if (entity.hasComponent<component::Sound>())
        entityStopAllSound(entity);
}

void FckGame::onWorldEntityDestroyed(const Entity &entity)
{
    if (entity.hasComponent<component::Script>())
    {
        component::Script &script_component = entity.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityDestroyed(entity);
    }
}

void FckGame::entityMove(const Entity &entity, const sf::Vector2f &offset)
{
    component::Transform &transform_component = entity.getComponent<component::Transform>();
    transform_component.transform.move(offset);

    if (!transform_component.children.empty())
    {
        for (auto it = transform_component.children.begin();
             it != transform_component.children.end();)
        {
            if (it->isValid())
            {
                component::Transform &child_transform_component
                    = it->getComponent<component::Transform>();
                entitySetPosition(*it, transform_component.transform.getPosition());
                ++it;
            }
            else
            {
                it = transform_component.children.erase(it);
            }
        }
    }

    if (entity.hasComponent<component::Scene>())
        m_scene_system.moveEntity(entity, offset);

    if (entity.hasComponent<component::Drawable>())
        m_render_system.moveEntity(entity, offset);

    if (entity.hasComponent<component::Script>())
    {
        component::Script &script_component = entity.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityMoved(entity, offset);
    }

    if (entity.hasComponent<component::LookAround>())
        m_look_around_system.updateBounds(entity);

    // sounds
    if (entity == m_player_entity)
    {
        sf::Listener::setPosition(
            {transform_component.transform.getPosition().x,
             0,
             transform_component.transform.getPosition().y});
    }

    if (entity.hasComponent<component::Sound>())
    {
        component::Sound &sound_component = entity.getComponent<component::Sound>();
        for (auto &it : sound_component.sounds)
        {
            it.second->setPosition(sf::Vector3f{
                transform_component.transform.getPosition().x,
                0,
                transform_component.transform.getPosition().y});
        }
    }

    // get tile material under entity
    if (m_level && m_level->getCurrentRoomCoord() != sf::Vector2i{-1, -1})
    {
        tile_material_type::Type type
            = m_level->getRoomsMap()
                  .getData(m_level->getCurrentRoomCoord())
                  ->getTileMaterials()
                  .getDataByPosition(transform_component.transform.getPosition());

        entitySetTileMaterial(entity, type);
    }
}

void FckGame::entitySetPosition(const Entity &entity, const sf::Vector2f &position)
{
    component::Transform &transform_component = entity.getComponent<component::Transform>();
    entityMove(entity, position - transform_component.transform.getPosition());
}

void FckGame::entitySetParent(const Entity &entity, const Entity &parent)
{
    component::Transform &transform_component = entity.getComponent<component::Transform>();

    if (transform_component.parent == parent)
        return;

    if (transform_component.parent.isValid())
    {
        component::Transform &parent_transform_component
            = transform_component.parent.getComponent<component::Transform>();

        parent_transform_component.children.erase(
            std::remove(
                parent_transform_component.children.begin(),
                parent_transform_component.children.end(),
                entity),
            parent_transform_component.children.end());
    }

    transform_component.parent = parent;

    if (transform_component.parent.isValid())
    {
        component::Transform &parent_transform_component
            = transform_component.parent.getComponent<component::Transform>();
        parent_transform_component.children.push_back(entity);
        entity::set_position.emit(entity, parent_transform_component.transform.getPosition());
    }
}

void FckGame::entitySetState(const Entity &entity, entity_state::State state)
{
    component::State &state_component = entity.getComponent<component::State>();

    if (state_component.state == state)
        return;

    state_component.state = state;

    if (state_component.state == entity_state::DEATH && entity.hasComponent<component::Velocity>())
    {
        component::Velocity &velocity_component = entity.getComponent<component::Velocity>();
        velocity_component.velocity = {};
    }

    if (entity.hasComponent<component::Script>())
    {
        component::Script &script_component = entity.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityStateChanged(entity, state);
    }
}

void FckGame::entitySetDirection(const Entity &entity, entity_state::Direction direction)
{
    component::State &state_component = entity.getComponent<component::State>();

    if (state_component.direction == direction)
        return;

    state_component.direction = direction;

    if (entity.hasComponent<component::Transform>())
    {
        component::Transform &transform_component = entity.getComponent<component::Transform>();
        transform_component.transform.setScale(
            {float(state_component.direction)
                 * std::abs(transform_component.transform.getScale().x),
             transform_component.transform.getScale().y});
    }

    if (entity.hasComponent<component::LookAround>())
        m_look_around_system.updateBounds(entity);

    if (entity.hasComponent<component::Script>())
    {
        component::Script &script_component = entity.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityDirectionChanged(entity, direction);
    }
}

void FckGame::entitySetTarget(const Entity &entity, const Entity &target)
{
    component::Target &target_component = entity.getComponent<component::Target>();

    if (target_component.target == target)
        return;

    Entity old_target = target_component.target;
    target_component.target = target;

    if (old_target.isValid())
    {
        component::Target &old_target_target_component
            = old_target.getComponent<component::Target>();

        old_target_target_component.lookings.erase(
            std::remove(
                old_target_target_component.lookings.begin(),
                old_target_target_component.lookings.end(),
                entity),
            old_target_target_component.lookings.end());
    }

    if (target_component.target.isValid())
    {
        component::Target &target_target_component
            = target_component.target.getComponent<component::Target>();
        target_target_component.lookings.push_back(entity);
    }

    // Set marker
    if (entity == m_player_entity)
    {
        if (old_target.isValid())
            entitySetMarker(old_target, Entity{});

        if (target_component.target.isValid())
            entitySetMarker(
                target_component.target, KnowledgeBase::createEntity("target", &m_world));
    }

    // Update target stats gui
    if (entity == m_player_entity && m_state == game_state::LEVEL)
    {
        m_gui_manager.getBack<gui::LevelGui>()->updateTargetStats();
    }
}

void FckGame::entitySetMarker(const Entity &entity, const Entity &marker)
{
    component::Marker &marker_component = entity.getComponent<component::Marker>();

    if (marker_component.marker == marker)
        return;

    if (marker_component.marker.isValid())
        entityDestroy(marker_component.marker);

    marker_component.marker = marker;

    if (marker_component.marker.isValid())
    {
        entitySetParent(marker_component.marker, entity);
        if (!marker_component.marker.isEnabled())
            marker_component.marker.enable();
    }
}

void FckGame::entitySetHealth(const Entity &entity, float health)
{
    component::Stats &stats_component = entity.getComponent<component::Stats>();

    stats_component.health = health;

    if (stats_component.health < 0)
        stats_component.health = 0;

    if (stats_component.health > stats_component.max_health)
        stats_component.health = stats_component.max_health;

    // Update stats gui
    if (m_state == game_state::LEVEL)
    {
        if (m_player_entity.isValid())
        {
            if (entity == m_player_entity)
            {
                m_gui_manager.getBack<gui::LevelGui>()->updatePlayerStats();
            }
            else if (entity == m_player_entity.getComponent<component::Target>().target)
            {
                m_gui_manager.getBack<gui::LevelGui>()->updateTargetStats();
            }
        }
    }
}

void FckGame::entitySetArmor(const Entity &entity, float armor)
{
    component::Stats &stats_component = entity.getComponent<component::Stats>();

    stats_component.armor = armor;

    if (stats_component.armor < 0)
        stats_component.armor = 0;

    if (stats_component.armor > stats_component.max_armor)
        stats_component.armor = stats_component.max_armor;

    // Update stats gui
    if (m_state == game_state::LEVEL)
    {
        if (m_player_entity.isValid())
        {
            if (entity == m_player_entity)
            {
                m_gui_manager.getBack<gui::LevelGui>()->updatePlayerStats();
            }
            else if (entity == m_player_entity.getComponent<component::Target>().target)
            {
                m_gui_manager.getBack<gui::LevelGui>()->updateTargetStats();
            }
        }
    }
}

void FckGame::entityDestroy(const Entity &entity)
{
    if (entity.hasComponent<component::Transform>())
        entitySetParent(entity, Entity{});

    if (entity.hasComponent<component::Target>())
        entitySetTarget(entity, Entity{});

    if (entity.hasComponent<component::Marker>())
        entitySetMarker(entity, Entity{});

    m_world.destroyEntity(entity);
}

void FckGame::entityCollided(const Entity &entity, const Entity &other)
{
    if (entity.hasComponent<component::Script>())
    {
        component::Script &script_component = entity.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityCollided(entity, other);
    }

    if (other.hasComponent<component::Script>())
    {
        component::Script &script_component = other.getComponent<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityCollided(other, entity);
    }
}

void FckGame::entityDrawableSetState(const Entity &entity, const std::string &state)
{
    if (entity.hasComponent<component::DrawableState>())
    {
        component::DrawableState &drawable_state_component
            = entity.getComponent<component::DrawableState>();

        if (drawable_state_component.state)
            drawable_state_component.state->setCurrentState(state);
    }

    if (entity.hasComponent<component::DrawableAnimation>())
    {
        component::DrawableAnimation &drawable_animation_component
            = entity.getComponent<component::DrawableAnimation>();

        if (drawable_animation_component.animation)
        {
            drawable_animation_component.animation->setCurrentState(state);
            drawable_animation_component.animation->start();
        }
    }
}

void FckGame::entityPlaySound(const Entity &entity, const std::string &sound_name)
{
    if (!entity.hasComponent<component::Sound>())
        return;

    component::Sound &sound_component = entity.getComponent<component::Sound>();

    if (sound_name == entity_state::stateToString(entity_state::MOVE))
    {
        for (auto &it : sound_component.sounds)
        {
            if (it.first.ends_with(sound_name))
                it.second->stop();
        }

        std::string move_sound_name
            = tile_material_type::toString(sound_component.tile_material) + "_move";

        auto sounds_found = sound_component.sounds.find(move_sound_name);
        if (sounds_found == sound_component.sounds.end())
            move_sound_name = "move";

        sounds_found = sound_component.sounds.find(move_sound_name);
        if (sounds_found != sound_component.sounds.end())
            sounds_found->second->play();
    }
    else
    {
        auto sounds_found = sound_component.sounds.find(sound_name);
        if (sounds_found != sound_component.sounds.end())
            sounds_found->second->play();
    }
}

void FckGame::entityStopSound(const Entity &entity, const std::string &sound_name)
{
    if (!entity.hasComponent<component::Sound>())
        return;

    component::Sound &sound_component = entity.getComponent<component::Sound>();

    if (sound_name == entity_state::stateToString(entity_state::MOVE))
    {
        for (auto &it : sound_component.sounds)
        {
            if (it.first.ends_with(sound_name))
                it.second->stop();
        }
    }
    else
    {
        auto sounds_found = sound_component.sounds.find(sound_name);
        if (sounds_found != sound_component.sounds.end())
            sounds_found->second->stop();
    }
}

void FckGame::entityStopAllSound(const Entity &entity)
{
    if (!entity.hasComponent<component::Sound>())
        return;

    component::Sound &sound_component = entity.getComponent<component::Sound>();

    for (auto &it : sound_component.sounds)
        it.second->stop();
}

void FckGame::entitySetTileMaterial(const Entity &entity, tile_material_type::Type tile_material)
{
    if (entity.hasComponent<component::Sound>())
    {
        component::Sound &sound_component = entity.getComponent<component::Sound>();

        if (tile_material != sound_component.tile_material)
        {
            sound_component.tile_material = tile_material;

            if (entity.hasComponent<component::State>())
            {
                component::State &state_component = entity.getComponent<component::State>();
                if (state_component.state == entity_state::MOVE)
                    entityPlaySound(entity, entity_state::stateToString(entity_state::MOVE));
            }
        }
    }
}

void FckGame::onLevelRoomOpened(const sf::Vector2i &room_coord)
{
    if (m_state == game_state::LEVEL)
    {
        m_gui_manager.getBack<gui::LevelGui>()->setRoomOpended(room_coord);
    }
}

void FckGame::onLevelRoomEnabled(const sf::Vector2i &room_coord)
{
    m_target_follow_system.setWalls(&m_level->getRoomsMap().getData(room_coord)->getWalls());

    if (m_state == game_state::LEVEL)
    {
        m_gui_manager.getBack<gui::LevelGui>()->setCurrentRoom(room_coord);
    }
}

} // namespace fck
