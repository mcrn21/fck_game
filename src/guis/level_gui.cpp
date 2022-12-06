#include "level_gui.h"

#include "../components/components.h"

#include "../fck/clipping.h"
#include "../fck/resource_cache.h"
#include "../fck/utilities.h"

namespace fck
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
    m_player_hp_progress_bar.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_player_hp_progress_bar.setBackgroundTextureRect(sf::IntRect{{0, 0}, {76, 8}});
    m_player_hp_progress_bar.setBarTextureRect(sf::IntRect{{2, 25}, {68, 3}});
    m_player_hp_progress_bar.setBarPosition({2.0f, 2.0f});
    m_player_hp_progress_bar.setScale(m_stats_scale);

    m_player_hp_progress_bar.text().setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_player_hp_progress_bar.text().setCharacterSize(24);
    m_player_hp_progress_bar.text().setOutlineColor(sf::Color::Black);
    m_player_hp_progress_bar.text().setOutlineThickness(1.0f);
    m_player_hp_progress_bar.text().setScale(vector2::div({1.0f, 1.0f}, m_stats_scale));
    m_player_hp_progress_bar.text().setPosition({4.0f, -2.0f});

    m_player_armor_progress_bar.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_player_armor_progress_bar.setBackgroundTextureRect(sf::IntRect{{0, 8}, {56, 8}});
    m_player_armor_progress_bar.setBarTextureRect(sf::IntRect{{2, 33}, {52, 3}});
    m_player_armor_progress_bar.setBarPosition({2.0f, 2.0f});
    m_player_armor_progress_bar.setScale(m_stats_scale);

    m_player_armor_progress_bar.text().setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_player_armor_progress_bar.text().setCharacterSize(24);
    m_player_armor_progress_bar.text().setOutlineColor(sf::Color::Black);
    m_player_armor_progress_bar.text().setOutlineThickness(1.0f);
    m_player_armor_progress_bar.text().setScale(vector2::div({1.0f, 1.0f}, m_stats_scale));
    m_player_armor_progress_bar.text().setPosition({4.0f, -2.0f});

    m_minimap_sprite.setTexture(ResourceCache::resource<sf::Texture>("ui"));
    m_minimap_sprite.setTextureRect({{0, 40}, {32, 32}});
    m_minimap_sprite.setScale(m_stats_scale);

    m_room_texture_rects.emplace(Level::Room::UNKNOW, sf::IntRect{{51, 40}, {11, 11}});
    m_room_texture_rects.emplace(Level::Room::DEFAULT, sf::IntRect{{40, 40}, {11, 11}});

    updatePlayerStats();

    // target
    m_target_hp_progress_bar.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_target_hp_progress_bar.setBackgroundTextureRect(sf::IntRect{{0, 0}, {76, 8}});
    m_target_hp_progress_bar.setBarTextureRect(sf::IntRect{{2, 25}, {68, 3}});
    m_target_hp_progress_bar.setBarPosition({2.0f, 2.0f});
    m_target_hp_progress_bar.setScale({-m_stats_scale.x, m_stats_scale.y});

    m_target_hp_progress_bar.text().setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_target_hp_progress_bar.text().setCharacterSize(24);
    m_target_hp_progress_bar.text().setOutlineColor(sf::Color::Black);
    m_target_hp_progress_bar.text().setOutlineThickness(1.0f);
    m_target_hp_progress_bar.text().setScale(vector2::div({-1.0f, 1.0f}, m_stats_scale));
    m_target_hp_progress_bar.text().setPosition({4.0f, -2.0f});

    updateTargetStats();

    updatePlayerSkills();
}

void LevelGui::resize(const sf::Vector2f &size)
{
    m_size = size;

    // player
    m_player_hp_progress_bar.setPosition(m_border_offset);
    m_player_armor_progress_bar.setPosition(
        m_border_offset + vector2::mult(sf::Vector2f{0.0f, 8.0f}, m_stats_scale));

    m_minimap_sprite.setPosition(
        m_border_offset + vector2::mult(sf::Vector2f{0.0f, 16.0f}, m_stats_scale));

    sf::Vector2f minimap_sprite_global_center
        = m_minimap_sprite.getPosition() + m_minimap_sprite.globalBounds().getSize() / 2.0f;
    for (auto &room_sprite : m_room_minimap_sprites)
    {
        room_sprite.second.setPosition(
            minimap_sprite_global_center
            + vector2::mult(
                room_sprite.second.globalBounds().getSize(), sf::Vector2f{room_sprite.first}));
    }

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
    m_target_hp_progress_bar.setPosition({m_size.x - m_border_offset.x, m_border_offset.y});
}

void LevelGui::draw(sf::RenderTarget &target, const sf::RenderStates &states)
{
    if (!m_player_entity.isValid())
        return;

    drawPlayerStats(target, states);
    drawPlayerSkills(target, states);

    TargetComponent &target_component = m_player_entity.component<TargetComponent>();
    if (target_component.target.isValid())
        drawTargetStats(target, states);
}

void LevelGui::updatePlayerStats()
{
    StatsComponent &stats_component = m_player_entity.component<StatsComponent>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));

    std::string armor_text = std::to_string(int32_t(stats_component.armor)) + "/"
        + std::to_string(int32_t(stats_component.max_armor));

    float health_ratio = stats_component.health / stats_component.max_health;
    float armor_ratio = stats_component.armor / stats_component.max_armor;

    m_player_hp_progress_bar.setProgress(health_ratio);
    m_player_hp_progress_bar.text().setString(hp_text);

    m_player_armor_progress_bar.setProgress(armor_ratio);
    m_player_armor_progress_bar.text().setString(armor_text);
}

void LevelGui::updatePlayerSkills()
{
    SkillsComponent &skills_component = m_player_entity.component<SkillsComponent>();
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
    TargetComponent &target_component = m_player_entity.component<TargetComponent>();
    if (!target_component.target.isValid())
        return;

    StatsComponent &stats_component = target_component.target.component<StatsComponent>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));

    float health_ratio = stats_component.health / stats_component.max_health;
    m_target_hp_progress_bar.setProgress(health_ratio);

    m_target_hp_progress_bar.text().setString(hp_text);
    m_target_hp_progress_bar.text().setOrigin(
        {m_target_hp_progress_bar.text().getLocalBounds().width, 0.0f});
}

void LevelGui::updateRoomsMinimap(
    const std::vector<std::pair<sf::Vector2i, Level::Room::Type>> &rooms)
{
    m_room_minimap_sprites.clear();

    sf::Vector2f position
        = m_minimap_sprite.getPosition() + m_minimap_sprite.globalBounds().getSize() / 2.0f;

    for (const auto &room : rooms)
    {
        Sprite room_sprite;
        room_sprite.setTexture(ResourceCache::resource<sf::Texture>("ui"));
        room_sprite.setTextureRect(m_room_texture_rects.at(room.second));
        room_sprite.setOrigin(room_sprite.localBounds().getSize() / 2.0f);
        room_sprite.setScale(m_stats_scale);
        room_sprite.setPosition(
            position
            + vector2::mult(room_sprite.globalBounds().getSize(), sf::Vector2f{room.first}));

        m_room_minimap_sprites.emplace_back(room.first, room_sprite);

        if (room.first.x == 0 && room.first.y == 0)
        {
            Sprite highlight_sprite;
            highlight_sprite.setTexture(ResourceCache::resource<sf::Texture>("ui"));
            highlight_sprite.setTextureRect({{40, 62}, {11, 11}});
            highlight_sprite.setOrigin(highlight_sprite.localBounds().getSize() / 2.0f);
            highlight_sprite.setScale(m_stats_scale);
            highlight_sprite.setPosition(position);
            m_room_minimap_sprites.emplace_back(room.first, highlight_sprite);
        }
    }
}

void LevelGui::drawPlayerStats(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_player_hp_progress_bar);
    target.draw(m_player_armor_progress_bar);

    target.draw(m_minimap_sprite);

    Clipping clipping(
        target,
        states,
        m_minimap_sprite.getPosition() + vector2::mult({2.0f, 2.0f}, m_stats_scale),
        m_minimap_sprite.globalBounds().getSize() - vector2::mult({4.0f, 4.0f}, m_stats_scale));

    for (const auto &room_sprite : m_room_minimap_sprites)
        target.draw(room_sprite.second);
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
    target.draw(m_target_hp_progress_bar);
}

} // namespace fck
