#include "level_gui.h"
#include "gui_style.h"

#include "../components/components.h"

#include "../fck/clipping.h"
#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck::gui
{

LevelGui::LevelGui(const sf::Vector2f &size, const Entity &player_entity)
    : m_size{size}, m_player_entity(player_entity)
{
    m_border_offset = {20.0f, 40.0f};

    m_scale = {1.0f, 1.0f};
    m_stats_scale = {4.0f, 4.0f};

    m_stats_hp_bar_width = 68.0f;
    m_stats_armor_bar_width = 52.0f;

    // player
    m_player_hp_progress_bar.reset(GuiStyle::createPlayerHpBar());
    m_player_armor_progress_bar.reset(GuiStyle::createPlayerArmorBar());
    m_minimap.reset(GuiStyle::createMinimap());

    updatePlayerStats();

    // target
    m_target_hp_progress_bar.reset(GuiStyle::createTargetHpBar());

    updateTargetStats();

    updatePlayerSkills();
}

void LevelGui::resize(const sf::Vector2f &size)
{
    m_size = size;

    // player
    m_player_hp_progress_bar->setPosition(GuiStyle::viewport_offset);
    m_player_armor_progress_bar->setPosition(
        GuiStyle::viewport_offset + vector2::mult(sf::Vector2f{0.0f, 8.0f}, GuiStyle::stats_scale));
    m_minimap->setPosition(
        GuiStyle::viewport_offset
        + vector2::mult(sf::Vector2f{0.0f, 16.0f}, GuiStyle::stats_scale));

    // skills
    if (!m_skills.empty())
    {
        sf::Vector2f skill_icon_size = m_skills.front().sprite.getLocalBounds().getSize();

        for (int32_t i = 0; i < m_skills.size(); ++i)
        {
            float space = i == 0 ? 0.0f : 4.0f * m_stats_scale.x;
            m_skills[m_skills.size() - 1 - i].sprite.setPosition(
                m_size - m_border_offset
                - sf::Vector2f{skill_icon_size.x * m_stats_scale.x + space, 0} * float(i));
            m_skills[m_skills.size() - 1 - i].sprite.setScale(m_stats_scale);
            m_skills[m_skills.size() - 1 - i].key_text.setPosition(
                m_size - m_border_offset
                - sf::Vector2f{(skill_icon_size.x * m_stats_scale.x + space) * float(i), 4.0f});
        }
    }

    // target
    m_target_hp_progress_bar->setPosition(
        {m_size.x - GuiStyle::viewport_offset.x, GuiStyle::viewport_offset.y});
}

void LevelGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!m_player_entity.isValid())
        return;

    drawPlayerStats(target, states);
    drawPlayerSkills(target, states);

    component::Target &target_component = m_player_entity.component<component::Target>();
    if (target_component.target.isValid())
        drawTargetStats(target, states);
}

void LevelGui::updatePlayerStats()
{
    component::Stats &stats_component = m_player_entity.component<component::Stats>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));

    std::string armor_text = std::to_string(int32_t(stats_component.armor)) + "/"
        + std::to_string(int32_t(stats_component.max_armor));

    float health_ratio = stats_component.health / stats_component.max_health;
    float armor_ratio = stats_component.armor / stats_component.max_armor;

    m_player_hp_progress_bar->setProgress(health_ratio);
    m_player_hp_progress_bar->text().setString(hp_text);

    m_player_armor_progress_bar->setProgress(armor_ratio);
    m_player_armor_progress_bar->text().setString(armor_text);
}

void LevelGui::updatePlayerSkills()
{
    component::Skills &skills_component = m_player_entity.component<component::Skills>();
    m_skills.clear();

    for (const auto &skill : skills_component.skills)
    {
        KnowledgeBase::SkillItemBase *skill_item = KnowledgeBase::skill(skill->name());
        if (skill_item)
        {
            Skill s;

            s.skill_item = skill_item;
            s.skill = skill.get();

            s.sprite.setTexture(*ResourceCache::resource<sf::Texture>(skill_item->textureName()));
            s.sprite.setTextureRect(skill_item->textureRect());
            s.sprite.setOrigin(s.sprite.getLocalBounds().getSize());

            s.key_text.setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
            s.key_text.setString("J");
            s.key_text.setCharacterSize(24);
            s.key_text.setOutlineColor(sf::Color::Black);
            s.key_text.setOutlineThickness(1.0f);
            s.key_text.setOrigin(
                {s.key_text.getLocalBounds().width / 2
                     + s.sprite.getLocalBounds().width * m_stats_scale.x / 2,
                 0.0f});

            m_skills.push_back(s);
        }
    }

    resize(m_size);
}

void LevelGui::updateTargetStats()
{
    component::Target &target_component = m_player_entity.component<component::Target>();
    if (!target_component.target.isValid())
        return;

    component::Stats &stats_component = target_component.target.component<component::Stats>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));

    float health_ratio = stats_component.health / stats_component.max_health;
    m_target_hp_progress_bar->setProgress(health_ratio);

    m_target_hp_progress_bar->text().setString(hp_text);
    m_target_hp_progress_bar->text().setOrigin(
        {m_target_hp_progress_bar->text().getLocalBounds().width, 0.0f});
}

void LevelGui::setRoomsMap(const Vector2D<Level::Room *> &rooms_map)
{
    m_minimap->setRoomsMap(rooms_map);
}

void LevelGui::setRoomOpended(const sf::Vector2i &room_coord)
{
    m_minimap->setRoomOpened(room_coord);
}

void LevelGui::setCurrentRoom(const sf::Vector2i &room_coord)
{
    m_minimap->setCurrentRoom(room_coord);
}

void LevelGui::drawPlayerStats(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(*m_player_hp_progress_bar);
    target.draw(*m_player_armor_progress_bar);
    target.draw(*m_minimap);
}

void LevelGui::drawPlayerSkills(sf::RenderTarget &target, const sf::RenderStates &states)
{
    for (auto &skill : m_skills)
    {
        skill.sprite.setColor(
            skill.skill->isReady() ? sf::Color::White : sf::Color(255, 255, 255, 127));
        target.draw(skill.sprite);
        target.draw(skill.key_text);
    }
}

void LevelGui::drawTargetStats(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(*m_target_hp_progress_bar);
}

} // namespace fck::gui
