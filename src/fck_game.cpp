#include "fck_game.h"
#include "debug_draw.h"
#include "entity_utils.h"
#include "events.h"
#include "knowledge_base.h"
#include "settings.h"

#include "components/components.h"
#include "damages/damages.h"
#include "entity_scripts/entity_scripts.h"
#include "map/factory.h"
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
            event_type::EXIT_GAME,
            event_type::NEW_GAME,
            event_type::RETURN_TO_GAME,
            event_type::RETURN_TO_MAIN_MENU},
        std::bind(
            static_cast<void (FckGame::*)(Event *)>(&FckGame::event), this, std::placeholders::_1));

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

    // sound
    entity::play_sound.connect(this, &FckGame::entityPlaySound);
    entity::stop_sound.connect(this, &FckGame::entityStopSound);
    entity::stop_all_sound.connect(this, &FckGame::entityStopAllSound);

    // skills
    entity::skill_applied.connect(this, &FckGame::onEntitySkillApplied);
    entity::skill_finished.connect(this, &FckGame::onEntitySkillFinished);

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

    m_main_widget.setRenderWindow(getRrenderWindow());

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
             gui::LoadingWidget *loading_widget
                 = static_cast<gui::LoadingWidget *>(m_main_widget.getChildren().back());
             loading_widget->setTotal(first_loading_tasks->getTasks().size() - 2);
             first_loading_tasks->task_finished.connect(loading_widget, &gui::LoadingWidget::next);
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

void FckGame::event(const sf::Event &e)
{
    m_main_widget.event(e);
    m_input_actions.event(e);

    if (e.type == sf::Event::Resized)
    {
        float scene_view_scale = 0.16;

        m_scene_view.setSize(sf::Vector2f(
            e.size.width * scene_view_scale + 0.5, e.size.height * scene_view_scale + 0.5));

        (void)(m_scene_render_texture.create(sf::Vector2u(e.size.width, e.size.height)));
        m_scene_render_texture.setSmooth(true);
        m_scene_render_sprite.setTexture(m_scene_render_texture.getTexture(), true);

        m_render_window_view.setSize(sf::Vector2f(e.size.width, e.size.height));
        m_render_window_view.setCenter(sf::Vector2f(e.size.width / 2, e.size.height / 2));

        getRrenderWindow().setView(m_render_window_view);
        Clipping::setOriginalView(m_render_window_view);

        return;
    }

    if (e.type == sf::Event::Closed)
        exit();
}

void FckGame::update(const sf::Time &elapsed)
{
    EventDispatcher::update(elapsed);

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
            component::Drawable &dca = a.get<component::Drawable>();
            component::Drawable &dcb = b.get<component::Drawable>();
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
            player_drawable_component = &m_player_entity.get<component::Drawable>();
            player_target_entity = m_player_entity.get<component::Target>().target;
        }

        for (Entity &entity : m_visible_entities)
        {
            if (entity.has<component::Drawable>())
            {
                component::Drawable &drawable_component = entity.get<component::Drawable>();
                component::Transform &transform_component = entity.get<component::Transform>();
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

                if (entity.has<component::Shadow>())
                {
                    component::Shadow &shadow_component = entity.get<component::Shadow>();
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

            //            if (m_map)
            //                debug_draw::drawMapWalls(m_map.get(), m_scene_render_texture);

            m_scene_tree.querry(viewport_rect, [this](int32_t node_id) {
                Entity entity = m_scene_tree.getUserData(node_id);

                debug_draw::drawDrawableBounds(entity, m_scene_render_texture);
                //                debug_draw::drawSceneTreeAABB(entity, m_scene_render_texture);
                debug_draw::drawSceneBounds(entity, m_scene_render_texture);
                debug_draw::drawVelocity(entity, m_scene_render_texture);

                if (m_map)
                    debug_draw::drawTargetFollowPath(entity, m_map.get(), m_scene_render_texture);

                debug_draw::drawLookAroundBound(entity, m_scene_render_texture);
                debug_draw::drawLookAroundLookBound(entity, m_scene_render_texture);

                return true;
            });
        }
    }

    m_scene_render_texture.display();

    getRrenderWindow().clear();

    getRrenderWindow().draw(m_scene_render_sprite);
    getRrenderWindow().draw(m_main_widget);

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
        gui::SplashWidget *splash_widget = new gui::SplashWidget{&m_main_widget};
        break;
    }
    case game_state::LOADING: {
        m_main_widget.clearChildren();
        gui::LoadingWidget *loading_widget = new gui::LoadingWidget{&m_main_widget};
        break;
    }
    case game_state::MAIN_MENU: {
        m_main_widget.clearChildren();
        gui::MainMenuWidget *main_menu_widget = new gui::MainMenuWidget{false, &m_main_widget};
        m_input_actions.action_activated.connect(
            main_menu_widget, &gui::MainMenuWidget::onActionActivated);
        break;
    }
    case game_state::LEVEL: {
        m_main_widget.removeLastChild();

        if (old_state != game_state::LEVEL_MENU)
        {
            gui::LevelWidget *level_widget = new gui::LevelWidget{&m_main_widget};
            level_widget->setPlayerEntity(m_player_entity);
        }

        m_input_actions.action_activated.connect(this, &FckGame::onActionActivated);
        m_input_actions.action_activated.connect(
            &m_player_actions_system, &system::PlayerActions::onActionActivated);
        m_input_actions.action_diactivated.connect(
            &m_player_actions_system, &system::PlayerActions::onActionDiactivated);

        break;
    }
    case game_state::LEVEL_MENU: {
        gui::MainMenuWidget *main_menu_widget = new gui::MainMenuWidget{true, &m_main_widget};
        m_input_actions.action_activated.connect(
            main_menu_widget, &gui::MainMenuWidget::onActionActivated);
        break;
    }
    default:
        break;
    }
}

void FckGame::event(Event *event)
{
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
    ResourceCache::loadFromFile<sf::Font>("font", settings->font_file_name);
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
             gui::LoadingWidget *loading_widget
                 = static_cast<gui::LoadingWidget *>(m_main_widget.getChildren().back());
             loading_widget->setTotal(loading_new_game_tasks->getTasks().size() - 2);
             loading_new_game_tasks->task_finished.connect(
                 loading_widget, &gui::LoadingWidget::next);

             //             m_level = std::make_unique<Level>(&m_world, &m_scene_tree);
             //             m_level->room_opened.connect(this, &FckGame::onLevelRoomOpened);
             //             m_level->room_enabled.connect(this, &FckGame::onLevelRoomEnabled);
             //             m_level->loadFromFile("resources/levels/l1.tmx");

             map::Factory map_factory{&m_world, &m_scene_tree};
             m_map.reset(map_factory.generateMapFromFile(30, "resources/levels/l1.tmx"));

             m_map->chunk_opened.connect(this, &FckGame::onMapChunkOpened);
             m_map->chunk_changed.connect(this, &FckGame::onMapChunkChanged);
         },
         [this]() {
             //             Vector2D<BoolProxy> rooms_map;
             //             rooms_map.resize({1, 1}, true);
             //             m_level->createRoomsMap(rooms_map);
             //             m_level->createRandomRoomsMap();

             //             m_level->createSingleRoom(room_side::LEFT);
         },
         [this]() { /*m_level->createRoomsContent();*/ },
         [this]() {
             m_view_movement_system.setMovementBounds(
                 {{0.0f, 0.0f}, sf::Vector2f(m_map->getAreaSize())});

             m_player_entity = KnowledgeBase::createPlayer("pp", &m_world);

             component::Script &player_entity_script_component
                 = m_player_entity.add<component::Script>();
             player_entity_script_component.entity_script.reset(new entity_script::Player());

             entityMove(m_player_entity, sf::Vector2f{100.0f, 100.0f});

             Entity kyoshi_2 = KnowledgeBase::createEntity("kyoshi_2", &m_world);
             entityMove(kyoshi_2, {150.0f, 150.0f});
             entitySetState(kyoshi_2, entity_state::IDLE);
             entitySetDirection(kyoshi_2, entity_state::RIGHT);
             kyoshi_2.enable();

             m_player_entity.enable();
         },
         [this, loading_new_game_tasks]() {
             loading_new_game_tasks->task_finished.disconnect();

             setState(game_state::LEVEL);

             gui::LevelWidget *level_widget
                 = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
             level_widget->setChunks(m_map->getChunks());

             m_map->setCurrentChunk(m_map->getFirstChunk(), {m_player_entity});

             //             m_level->enableRoom(m_level->getFirstRoomCoord(), {50.0f, 50.0f});

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
             gui::LoadingWidget *loading_widget
                 = static_cast<gui::LoadingWidget *>(m_main_widget.getChildren().back());
             loading_widget->setTotal(return_to_main_menu_tasks->getTasks().size() - 2);
             return_to_main_menu_tasks->task_finished.connect(
                 loading_widget, &gui::LoadingWidget::next);
             m_target_follow_system.clearWalls();
         },
         [this]() { m_map.reset(); },
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

void FckGame::createLevel(const std::string &level_name)
{
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
    if (entity.has<component::Script>())
    {
        component::Script &script_component = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityEnabled(entity);
    }
}

void FckGame::onWorldEntityDisabled(const Entity &entity)
{
    if (entity.has<component::Script>())
    {
        component::Script &script_component = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityDisabled(entity);
    }

    if (entity.has<component::Sound>())
        entityStopAllSound(entity);
}

void FckGame::onWorldEntityDestroyed(const Entity &entity)
{
    if (entity.has<component::Script>())
    {
        component::Script &script_component = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityDestroyed(entity);
    }
}

void FckGame::entityMove(const Entity &entity, const sf::Vector2f &offset)
{
    component::Transform &transform_component = entity.get<component::Transform>();
    transform_component.transform.move(offset);

    if (!transform_component.children.empty())
    {
        for (auto it = transform_component.children.begin();
             it != transform_component.children.end();)
        {
            if (it->isValid())
            {
                component::Transform &child_transform_component = it->get<component::Transform>();
                entitySetPosition(*it, transform_component.transform.getPosition());
                ++it;
            }
            else
            {
                it = transform_component.children.erase(it);
            }
        }
    }

    if (entity.has<component::Scene>())
        m_scene_system.moveEntity(entity, offset);

    if (entity.has<component::Drawable>())
        m_render_system.moveEntity(entity, offset);

    if (entity.has<component::LookAround>())
        m_look_around_system.updateBounds(entity);

    // grid
    entityUpdateGridPosition(entity);

    // update sound listener for player
    if (entity == m_player_entity)
    {
        sf::Listener::setPosition(
            {transform_component.transform.getPosition().x,
             0,
             transform_component.transform.getPosition().y});
    }

    // update entity sounds positions
    if (entity.has<component::Sound>())
    {
        component::Sound &sound_component = entity.get<component::Sound>();
        for (auto &it : sound_component.sounds)
        {
            it.second->setPosition(sf::Vector3f{
                transform_component.transform.getPosition().x,
                0,
                transform_component.transform.getPosition().y});
        }
    }

    if (entity.has<component::Script>())
    {
        component::Script &script_component = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityMoved(entity, offset);
    }
}

void FckGame::entitySetPosition(const Entity &entity, const sf::Vector2f &position)
{
    component::Transform &transform_component = entity.get<component::Transform>();
    entityMove(entity, position - transform_component.transform.getPosition());
}

void FckGame::entitySetParent(const Entity &entity, const Entity &parent)
{
    component::Transform &transform_component = entity.get<component::Transform>();

    if (transform_component.parent == parent)
        return;

    if (transform_component.parent.isValid())
    {
        component::Transform &parent_transform_component
            = transform_component.parent.get<component::Transform>();

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
            = transform_component.parent.get<component::Transform>();
        parent_transform_component.children.push_back(entity);
        entity::set_position.emit(entity, parent_transform_component.transform.getPosition());
    }
}

void FckGame::entitySetState(const Entity &entity, entity_state::State state)
{
    component::State &state_component = entity.get<component::State>();

    if (state_component.state == state)
        return;

    state_component.state = state;

    if (state_component.state == entity_state::DEATH && entity.has<component::Velocity>())
    {
        component::Velocity &velocity_component = entity.get<component::Velocity>();
        velocity_component.velocity = {};
    }

    if (entity.has<component::Script>())
    {
        component::Script &script_component = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityStateChanged(entity, state);
    }
}

void FckGame::entitySetDirection(const Entity &entity, entity_state::Direction direction)
{
    component::State &state_component = entity.get<component::State>();

    if (state_component.direction == direction)
        return;

    state_component.direction = direction;

    if (entity.has<component::Transform>())
    {
        component::Transform &transform_component = entity.get<component::Transform>();
        transform_component.transform.setScale(
            {float(state_component.direction)
                 * std::abs(transform_component.transform.getScale().x),
             transform_component.transform.getScale().y});
    }

    if (entity.has<component::LookAround>())
        m_look_around_system.updateBounds(entity);

    if (entity.has<component::Script>())
    {
        component::Script &script_component = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityDirectionChanged(entity, direction);
    }
}

void FckGame::entitySetTarget(const Entity &entity, const Entity &target)
{
    component::Target &target_component = entity.get<component::Target>();

    if (target_component.target == target)
        return;

    Entity old_target = target_component.target;
    target_component.target = target;

    if (old_target.isValid())
    {
        component::Target &old_target_target_component = old_target.get<component::Target>();

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
            = target_component.target.get<component::Target>();
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
        gui::LevelWidget *level_widget
            = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
        level_widget->setTargetEntity(target);
    }
}

void FckGame::entitySetMarker(const Entity &entity, const Entity &marker)
{
    component::Marker &marker_component = entity.get<component::Marker>();

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
    component::Stats &stats_component = entity.get<component::Stats>();

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
                gui::LevelWidget *level_widget
                    = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
                level_widget->updatePlayerStats();
            }
            else if (entity == m_player_entity.get<component::Target>().target)
            {
                gui::LevelWidget *level_widget
                    = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
                level_widget->updateTargetStats();
            }
        }
    }
}

void FckGame::entitySetArmor(const Entity &entity, float armor)
{
    component::Stats &stats_component = entity.get<component::Stats>();

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
                gui::LevelWidget *level_widget
                    = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
                level_widget->updatePlayerStats();
            }
            else if (entity == m_player_entity.get<component::Target>().target)
            {
                gui::LevelWidget *level_widget
                    = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
                level_widget->updateTargetStats();
            }
        }
    }
}

void FckGame::entityDestroy(const Entity &entity)
{
    if (entity.has<component::Transform>())
        entitySetParent(entity, Entity{});

    if (entity.has<component::Target>())
        entitySetTarget(entity, Entity{});

    if (entity.has<component::Marker>())
        entitySetMarker(entity, Entity{});

    m_world.destroyEntity(entity);
}

void FckGame::entityCollided(const Entity &entity, const Entity &other)
{
    if (entity.has<component::Script>())
    {
        component::Script &script_component = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityCollided(entity, other);
    }

    if (other.has<component::Script>())
    {
        component::Script &script_component = other.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->onEntityCollided(other, entity);
    }
}

void FckGame::entityDrawableSetState(const Entity &entity, const std::string &state)
{
    if (entity.has<component::DrawableState>())
    {
        component::DrawableState &drawable_state_component = entity.get<component::DrawableState>();

        if (drawable_state_component.state)
            drawable_state_component.state->setCurrentState(state);
    }

    if (entity.has<component::DrawableAnimation>())
    {
        component::DrawableAnimation &drawable_animation_component
            = entity.get<component::DrawableAnimation>();

        if (drawable_animation_component.animation)
        {
            drawable_animation_component.animation->setCurrentState(state);
            drawable_animation_component.animation->start();
        }
    }
}

void FckGame::entityPlaySound(const Entity &entity, const std::string &sound_name)
{
    if (!entity.has<component::Sound>())
        return;

    component::Sound &sound_component = entity.get<component::Sound>();

    if (sound_name == entity_state::stateToString(entity_state::MOVE))
    {
        for (auto &it : sound_component.sounds)
        {
            if (it.first.ends_with(sound_name))
                it.second->stop();
        }

        std::string move_sound_name;
        if (entity.has<component::Grid>())
        {
            auto &grid_component = entity.get<component::Grid>();
            move_sound_name = tile_material_type::toString(grid_component.tile_material) + "_move";
        }

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
    if (!entity.has<component::Sound>())
        return;

    component::Sound &sound_component = entity.get<component::Sound>();

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
    if (!entity.has<component::Sound>())
        return;

    component::Sound &sound_component = entity.get<component::Sound>();

    for (auto &it : sound_component.sounds)
        it.second->stop();
}

void FckGame::entityUpdateGridPosition(const Entity &entity)
{
    if (!entity.has<component::Grid>() || !m_level
        || m_level->getCurrentRoomCoord() == sf::Vector2i{-1, -1})
        return;

    auto &grid_component = entity.get<component::Grid>();
    sf::Vector2i grid_position
        = m_level->getRoomsMap()
              .getData(m_level->getCurrentRoomCoord())
              ->getTileMaterials()
              .transformPosition(entity.get<component::Transform>().transform.getPosition());

    if (grid_component.position == grid_position)
        return;

    grid_component.position = grid_position;

    auto old_tile_material = grid_component.tile_material;
    grid_component.tile_material = m_level->getRoomsMap()
                                       .getData(m_level->getCurrentRoomCoord())
                                       ->getTileMaterials()
                                       .getData(grid_component.position);

    // Update current playing sound
    if (entity.has<component::Sound>() && old_tile_material != grid_component.tile_material)
    {
        component::Sound &sound_component = entity.get<component::Sound>();
        if (entity.has<component::State>())
        {
            component::State &state_component = entity.get<component::State>();
            if (state_component.state == entity_state::MOVE)
                entityPlaySound(entity, entity_state::stateToString(entity_state::MOVE));
        }
    }
}

void FckGame::onEntitySkillApplied(const Entity &entity, SkillBase *skill)
{
    if (entity == m_player_entity)
    {
        gui::LevelWidget *level_widget
            = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
        level_widget->updatePlayerSkillStates();
    }
}

void FckGame::onEntitySkillFinished(const Entity &entity, SkillBase *skill)
{
    if (entity == m_player_entity)
    {
        gui::LevelWidget *level_widget
            = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
        level_widget->updatePlayerSkillStates();
    }
}

void FckGame::onMapChunkOpened(const sf::Vector2i &chunk_coords)
{
    gui::LevelWidget *level_widget
        = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
    level_widget->setChunkOpened(chunk_coords);
}

void FckGame::onMapChunkChanged(const sf::Vector2i &chunk_coords)
{
    // change walls in target follow system
    const map::Chunk *chunk = m_map->getChunks().getData(chunk_coords);
    m_target_follow_system.setWalls(chunk->getWalls());
    m_target_follow_system.setWallSize(chunk->getWallSize());

    gui::LevelWidget *level_widget
        = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
    level_widget->setCurrentChunk(chunk_coords);
}

} // namespace fck
