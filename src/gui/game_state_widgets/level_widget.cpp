#include "level_widget.h"

#include "../../components/components.h"

namespace fck::gui
{

LevelWidget::LevelWidget(Widget *parent) : Widget{parent}
{
    WidgetTheme widget_theme = getWidgetTheme();
    widget_theme.padding = 0.0f;
    setWidgetTheme(widget_theme);

    setSize(Widget::getWindowSize());
    setSelectable(false);

    // player stats
    m_player_hp_progress_bar = new ProgressBar{this};
    m_player_hp_progress_bar->setSize({256.0f, 28.0f});

    WidgetTheme player_hp_progress_bar = m_player_hp_progress_bar->getWidgetTheme();

    player_hp_progress_bar.padding = 0.0f; //{0.0f, 10.0f, 10.0f, 10.0f};

    player_hp_progress_bar.background.texture_rects[WidgetState::DEFAULT] = {{32, 32}, {32, 32}};
    player_hp_progress_bar.background.texture_rects[WidgetState::HOVERED] = {{32, 32}, {32, 32}};
    player_hp_progress_bar.background.texture_rects[WidgetState::PRESSED] = {{32, 32}, {32, 32}};
    player_hp_progress_bar.background.texture_rects[WidgetState::FOCUSED] = {{32, 32}, {32, 32}};

    player_hp_progress_bar.background.border_size = 12.0f;

    player_hp_progress_bar.foreground.border_size = 12.0f;

    player_hp_progress_bar.foreground.texture_rects[WidgetState::DEFAULT] = {{32, 96}, {32, 32}};
    player_hp_progress_bar.foreground.texture_rects[WidgetState::HOVERED] = {{32, 96}, {32, 32}};
    player_hp_progress_bar.foreground.texture_rects[WidgetState::PRESSED] = {{32, 96}, {32, 32}};
    player_hp_progress_bar.foreground.texture_rects[WidgetState::FOCUSED] = {{32, 96}, {32, 32}};

    player_hp_progress_bar.foreground.texture_border_size = 8;

    //    player_hp_progress_bar.text.fill_colors[WidgetState::DEFAULT] = sf::Color::Black;
    //    player_hp_progress_bar.text.fill_colors[WidgetState::HOVERED] = sf::Color::Black;
    //    player_hp_progress_bar.text.fill_colors[WidgetState::PRESSED] = sf::Color::Black;
    //    player_hp_progress_bar.text.fill_colors[WidgetState::FOCUSED] = sf::Color::Black;

    player_hp_progress_bar.text.character_size = 26;
    player_hp_progress_bar.text.align = TextAlign::LEFT | TextAlign::V_CENTER;

    m_player_hp_progress_bar->setTextOffset({10.0f, 0.0f});

    m_player_hp_progress_bar->setWidgetTheme(player_hp_progress_bar);

    m_player_armor_progress_bar = new ProgressBar{this};
    m_player_armor_progress_bar->setSize({208.0f, 28.0f});

    WidgetTheme player_armor_progress_bar
        = player_hp_progress_bar; //m_player_armor_progress_bar->getWidgetTheme();

    player_armor_progress_bar.foreground.texture_rects[WidgetState::DEFAULT] = {{64, 96}, {32, 32}};
    player_armor_progress_bar.foreground.texture_rects[WidgetState::HOVERED] = {{64, 96}, {32, 32}};
    player_armor_progress_bar.foreground.texture_rects[WidgetState::PRESSED] = {{64, 96}, {32, 32}};
    player_armor_progress_bar.foreground.texture_rects[WidgetState::FOCUSED] = {{64, 96}, {32, 32}};

    m_player_armor_progress_bar->setTextOffset({10.0f, 0.0f});

    m_player_armor_progress_bar->setWidgetTheme(player_armor_progress_bar);

    // minimap
    m_minimap = new Minimap{this};
    m_minimap->setSize({112.0f, 112.0f});

    // target stats
    m_target_hp_progress_bar = new ProgressBar{this};
    m_target_hp_progress_bar->setSize({256.0f, 28.0f});
    m_target_hp_progress_bar->hide();

    WidgetTheme target_hp_progress_bar
        = player_hp_progress_bar; //m_target_hp_progress_bar->getWidgetTheme();

    target_hp_progress_bar.text.align = TextAlign::RIGHT | TextAlign::V_CENTER;
    target_hp_progress_bar.direction = LEFT_TO_RIGHT;

    m_target_hp_progress_bar->setTextOffset({-10.0f, 0.0f});

    m_target_hp_progress_bar->setWidgetTheme(target_hp_progress_bar);

    updateGeometry();
}

const Entity &LevelWidget::getPlayerEntity() const
{
    return m_player_entity;
}

void LevelWidget::setPlayerEntity(const Entity &entity)
{
    m_player_entity = entity;
    updatePlayerStats();
}

const Entity &LevelWidget::getTargetEntity() const
{
    return m_target_entity;
}

void LevelWidget::setTargetEntity(const Entity &entity)
{
    m_target_entity = entity;

    if (m_target_entity.isValid())
    {
        m_target_hp_progress_bar->show();
        updateTargetStats();
    }
    else
    {
        m_target_hp_progress_bar->hide();
    }
}

void LevelWidget::updatePlayerStats()
{
    if (!m_player_entity.isValid())
        return;

    component::Stats &stats_component = m_player_entity.get<component::Stats>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));

    std::string armor_text = std::to_string(int32_t(stats_component.armor)) + "/"
        + std::to_string(int32_t(stats_component.max_armor));

    float health_ratio = stats_component.health / stats_component.max_health;
    float armor_ratio = stats_component.armor / stats_component.max_armor;

    m_player_hp_progress_bar->setProgress(health_ratio);
    m_player_hp_progress_bar->setString(hp_text);

    m_player_armor_progress_bar->setProgress(armor_ratio);
    m_player_armor_progress_bar->setString(armor_text);
}

void LevelWidget::updateTargetStats()
{
    if (!m_target_entity.isValid())
        return;

    component::Stats &stats_component = m_target_entity.get<component::Stats>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));

    float health_ratio = stats_component.health / stats_component.max_health;

    m_target_hp_progress_bar->setProgress(health_ratio);
    m_target_hp_progress_bar->setString(hp_text);
}

void LevelWidget::setRoomsMap(const Vector2D<Room *> &rooms_map)
{
    m_minimap->setRoomsMap(rooms_map);
}

void LevelWidget::setRoomOpended(const sf::Vector2i &room_coord)
{
    m_minimap->setRoomOpened(room_coord);
}

void LevelWidget::setCurrentRoom(const sf::Vector2i &room_coord)
{
    m_minimap->setCurrentRoom(room_coord);
}

void LevelWidget::onWindowResized(const sf::Vector2f &size)
{
    setSize(size);
    updateGeometry();
}

void LevelWidget::updateGeometry()
{
    Sides<float> viewport_padding = 30.0f;

    m_player_hp_progress_bar->setPosition(viewport_padding.getLowerPoint());
    m_player_armor_progress_bar->setPosition(
        viewport_padding.getLowerPoint() + sf::Vector2f{0.0f, 32.0f});

    m_minimap->setPosition(viewport_padding.getLowerPoint() + sf::Vector2f{0.0f, 64.0f});

    m_target_hp_progress_bar->setPosition(
        {getSize().x - viewport_padding.right - m_target_hp_progress_bar->getSize().x,
         viewport_padding.top});
}

} // namespace fck::gui
