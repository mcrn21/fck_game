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

    player_hp_progress_bar.text.align = TextAlign::LEFT | TextAlign::V_CENTER;

    m_player_hp_progress_bar->setTextOffset({10.0f, 0.0f});

    m_player_hp_progress_bar->setWidgetTheme(player_hp_progress_bar);

    m_player_armor_progress_bar = new ProgressBar{this};
    m_player_armor_progress_bar->setSize({208.0f, 28.0f});

    WidgetTheme player_armor_progress_bar = player_hp_progress_bar;

    player_armor_progress_bar.foreground.texture_rects[WidgetState::DEFAULT] = {{64, 96}, {32, 32}};
    player_armor_progress_bar.foreground.texture_rects[WidgetState::HOVERED] = {{64, 96}, {32, 32}};
    player_armor_progress_bar.foreground.texture_rects[WidgetState::PRESSED] = {{64, 96}, {32, 32}};
    player_armor_progress_bar.foreground.texture_rects[WidgetState::FOCUSED] = {{64, 96}, {32, 32}};

    m_player_armor_progress_bar->setTextOffset({10.0f, 0.0f});

    m_player_armor_progress_bar->setWidgetTheme(player_armor_progress_bar);

    // minimap
    m_minimap = new Minimap{this};
    m_minimap->setSize({128.0f, 128.0f});

    // target stats
    m_target_hp_progress_bar = new ProgressBar{this};
    m_target_hp_progress_bar->setSize({256.0f, 28.0f});
    m_target_hp_progress_bar->hide();

    WidgetTheme target_hp_progress_bar = player_hp_progress_bar;

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
    updatePlayerSkills();
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

void LevelWidget::updatePlayerSkills()
{
    m_player_skill_icons.clear();

    if (!m_player_entity.isValid())
        return;

    component::Skills &skills_component = m_player_entity.get<component::Skills>();
    for (auto &skill : skills_component.skills)
    {
        SkillIcon *skill_icon = new SkillIcon{*skill, this};
        skill_icon->setActivated(!skill->isReady());
        m_player_skill_icons.push_back(std::unique_ptr<SkillIcon>(skill_icon));
    }

    updateGeometry();
}

void LevelWidget::updatePlayerSkillStates()
{
    for (auto &skill_icon : m_player_skill_icons)
        skill_icon->setActivated(!skill_icon->getSkill()->isReady());
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

void LevelWidget::setChunks(const Vector2D<map::Chunk *> &chunks)
{
    m_minimap->setChunks(chunks);
}

void LevelWidget::setChunkOpened(const sf::Vector2i &room_coord)
{
    m_minimap->setChunkOpened(room_coord);
}

void LevelWidget::setCurrentChunk(const sf::Vector2i &room_coord)
{
    m_minimap->setCurrentChunk(room_coord);
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

    float skill_x_offset = viewport_padding.right;
    for (int32_t i = m_player_skill_icons.size() - 1; i >= 0; --i)
    {
        m_player_skill_icons[i]->setPosition(
            {getSize().x - skill_x_offset - m_player_skill_icons[i]->getSize().x,
             getSize().y - viewport_padding.bottom - m_player_skill_icons[i]->getSize().y});
        skill_x_offset += m_player_skill_icons[i]->getSize().x + 8.0f;
    }

    m_target_hp_progress_bar->setPosition(
        {getSize().x - viewport_padding.right - m_target_hp_progress_bar->getSize().x,
         viewport_padding.top});
}

} // namespace fck::gui
