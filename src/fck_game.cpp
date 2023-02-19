#include "fck_game.h"
#include "bindings/bindings.h"
#include "components/components.h"
#include "damages/damages.h"
#include "debug_draw.h"
#include "entity_funcs.h"
#include "fck/clipping.h"
#include "fck/event_dispatcher.h"
#include "fck/resource_cache.h"
#include "fck/sprite_animation.h"
#include "fck/task_sequence.h"
#include "fck/tile_map.h"
#include "knowledge_base/knowledge_base.h"
#include "map/factory.h"
#include "settings.h"
#include "skills/skills.h"

#include <spdlog/spdlog.h>

#include <SFML/Audio.hpp>

#include <algorithm>
#include <filesystem>

namespace fck
{

FckGame::FckGame()
    : BaseGame{},
      m_render_window_view{sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1, 1))},
      m_scene_view{sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(1, 1))},
      m_script_system{&m_lua_state},
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
    m_world.addSystem(m_player_system);
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
    m_world.addSystem(m_sound_system);

    // world
    m_world.entity_enabled.connect(&system::Script::onEntityEnabled, &m_script_system);
    m_world.entity_disabled.connect(&system::Script::onEntityDisabled, &m_script_system);
    m_world.entity_destroyed.connect(&system::Script::onEntityDestroyed, &m_script_system);
    m_world.entity_destroyed.connect([](const Entity &entity) {
        if (entity.has<component::Transform>())
            entity_funcs::setParent(entity, Entity{});

        if (entity.has<component::Target>())
            entity_funcs::setTarget(entity, Entity{});

        if (entity.has<component::Marker>())
            entity_funcs::setMarker(entity, Entity{});
    });

    // transform
    entity_funcs::moved.connect(&system::Scene::onEntityMoved, &m_scene_system);
    entity_funcs::moved.connect(&system::Render::onEntityMoved, &m_render_system);
    entity_funcs::moved.connect([this](const Entity &entity, const sf::Vector2f &) {
        m_look_around_system.updateBounds(entity);
    });
    entity_funcs::moved.connect(&system::Script::onEntityMoved, &m_script_system);
    entity_funcs::moved.connect(&system::Sound::onEntityMoved, &m_sound_system);

    // state
    entity_funcs::state_changed.connect(&system::Script::onEntityStateChanged, &m_script_system);
    entity_funcs::direction_changed.connect(
        &system::Render::onEntityDirectionChanged, &m_render_system);
    entity_funcs::direction_changed.connect([this](const Entity &entity, entity_state::Direction) {
        m_look_around_system.updateBounds(entity);
    });
    entity_funcs::direction_changed.connect(
        &system::Script::onEntityDirectionChanged, &m_script_system);

    // collided
    entity_funcs::collided.connect(&system::Script::onEntityCollided, &m_script_system);

    // target
    entity_funcs::target_changed.connect(&system::Player::onEntityTargetChanged, &m_player_system);

    // map
    map_changed.connect(&system::TargetFollow::onMapChanged, &m_target_follow_system);
    map_changed.connect(&system::Script::onMapChanged, &m_script_system);

    // lua
    m_lua_state.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io);
    bindToLua(m_lua_state);

    SkillFactory::setSolState(&m_lua_state);
    ScriptFactory::setSolState(&m_lua_state);
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
             first_loading_tasks->task_finished.connect(&gui::LoadingWidget::next, loading_widget);
             loadFonts();
         },
         [this]() { loadTextures(); },
         [this]() { loadSounds(); },
         [this]() {
             KnowledgeBase::loadSkillsFromDatabase(Settings::getGlobal()->resources_database_name);
         },
         [this]() {
             KnowledgeBase::loadScriptsFromDatabase(Settings::getGlobal()->resources_database_name);
         },
         [this]() {
             KnowledgeBase::loadEntitiesFromDatabase(
                 Settings::getGlobal()->resources_database_name);
         },
         [this]() { setupInputActions(); },
         [this, first_loading_tasks]() {
             first_loading_tasks->task_finished.disconnect_all();
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

        m_player_system.update(delta_time);
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

                if (drawable_component.shadow_shape)
                    m_scene_render_texture.draw(*drawable_component.shadow_shape, render_states);

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

    m_input_actions.action_activated.disconnect_all();

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
            &gui::MainMenuWidget::onActionActivated, main_menu_widget);
        break;
    }
    case game_state::LEVEL: {
        m_main_widget.removeLastChild();

        if (old_state != game_state::LEVEL_MENU)
        {
            gui::LevelWidget *level_widget = new gui::LevelWidget{&m_main_widget};
            level_widget->setPlayerEntity(m_player_entity);

            // signals
            entity_funcs::target_changed.connect(
                &gui::LevelWidget::onEntityTargetChanged, level_widget);
            entity_funcs::health_changed.connect(
                &gui::LevelWidget::onEntityHealthChanged, level_widget);
            entity_funcs::armor_changed.connect(
                &gui::LevelWidget::onEntityArmorChanged, level_widget);
            entity_funcs::skill_applied.connect(
                &gui::LevelWidget::onEntitySkillApplied, level_widget);
            entity_funcs::skill_finished.connect(
                &gui::LevelWidget::onEntitySkillFinished, level_widget);
        }

        m_input_actions.action_activated.connect(&FckGame::onActionActivated, this);
        m_input_actions.action_activated.connect(
            &system::Player::onActionActivated, &m_player_system);
        m_input_actions.action_diactivated.connect(
            &system::Player::onActionDiactivated, &m_player_system);

        break;
    }
    case game_state::LEVEL_MENU: {
        gui::MainMenuWidget *main_menu_widget = new gui::MainMenuWidget{true, &m_main_widget};
        m_input_actions.action_activated.connect(
            &gui::MainMenuWidget::onActionActivated, main_menu_widget);
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
                 &gui::LoadingWidget::next, loading_widget);

             //             m_level = std::make_unique<Level>(&m_world, &m_scene_tree);
             //             m_level->room_opened.connect(this, &FckGame::onLevelRoomOpened);
             //             m_level->room_enabled.connect(this, &FckGame::onLevelRoomEnabled);
             //             m_level->loadFromFile("resources/levels/l1.tmx");

             map::Factory map_factory{&m_world, &m_scene_tree};
             m_map.reset(map_factory.createMap(30, "resources/levels/l1.tmx"));

             map_changed(m_map.get());
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

             m_player_entity = EntityFactory::createPlayer("pp", &m_world);

             //             component::Script &player_entity_script_component
             //                 = m_player_entity.add<component::Script>();

             //player_entity_script_component.entity_script.reset(new entity_script::Player());

             entity_funcs::setPosition(m_player_entity, sf::Vector2f{100.0f, 100.0f});

             Entity kyoshi_2 = EntityFactory::createEntity("kyoshi_2", &m_world);
             entity_funcs::setPosition(kyoshi_2, {150.0f, 150.0f});
             entity_funcs::setState(kyoshi_2, entity_state::IDLE);
             entity_funcs::setDirection(kyoshi_2, entity_state::RIGHT);
             kyoshi_2.enable();

             m_player_entity.enable();
         },
         [this, loading_new_game_tasks]() {
             loading_new_game_tasks->task_finished.disconnect_all();

             setState(game_state::LEVEL);

             gui::LevelWidget *level_widget
                 = static_cast<gui::LevelWidget *>(m_main_widget.getChildren().back());
             level_widget->setChunks(m_map->getChunks());

             m_map->chunk_changed.connect(
                 &system::TargetFollow::onChunkChanged, &m_target_follow_system);
             m_map->chunk_changed.connect(&gui::LevelWidget::onChunkChanged, level_widget);
             m_map->chunk_opened.connect(&gui::LevelWidget::onChunkOpened, level_widget);

             m_map->setCurrentChunk(m_map->getFirstChunkCoords(), {m_player_entity});

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
                 &gui::LoadingWidget::next, loading_widget);
         },
         [this]() {
             m_map.reset();
             map_changed(nullptr);
         },
         [this]() { m_world.destroyAllEntities(); },
         [this]() { m_visible_entities.clear(); },
         [this, return_to_main_menu_tasks]() {
             return_to_main_menu_tasks->task_finished.disconnect_all();

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

} // namespace fck
