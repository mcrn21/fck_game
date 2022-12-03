#include "level_gui.h"

#include "../components/components.h"

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
    m_player_stats_bg_sprite.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_player_stats_bg_sprite.setTextureRect(sf::IntRect{{0, 0}, {76, 16}});
    m_player_stats_bg_sprite.scale(m_stats_scale);

    m_player_hp_bar_sprite.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_player_hp_bar_sprite.setTextureRect(sf::IntRect{{2, 25}, {68, 3}});
    m_player_hp_bar_sprite.scale(m_stats_scale);

    m_player_armor_bar_sprite.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_player_armor_bar_sprite.setTextureRect(sf::IntRect{{2, 33}, {52, 3}});
    m_player_armor_bar_sprite.scale(m_stats_scale);

    m_player_hp_text.setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_player_hp_text.setCharacterSize(24);
    m_player_hp_text.setOutlineColor(sf::Color::Black);
    m_player_hp_text.setOutlineThickness(1.0f);

    m_player_armor_text.setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_player_armor_text.setCharacterSize(24);
    m_player_armor_text.setOutlineColor(sf::Color::Black);
    m_player_armor_text.setOutlineThickness(1.0f);

    updatePlayerStats();

    // target
    m_target_stats_bg_sprite.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_target_stats_bg_sprite.setTextureRect(sf::IntRect{{0, 16}, {76, 8}});
    m_target_stats_bg_sprite.setOrigin({m_target_stats_bg_sprite.getLocalBounds().width, 0.0f});
    m_target_stats_bg_sprite.scale(m_stats_scale);

    m_target_hp_bar_sprite.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_target_hp_bar_sprite.setTextureRect(sf::IntRect{{2, 25}, {68, 3}});
    m_target_hp_bar_sprite.setOrigin({m_target_hp_bar_sprite.getLocalBounds().width, 0.0f});
    m_target_hp_bar_sprite.scale(m_stats_scale);

    m_target_hp_text.setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_target_hp_text.setCharacterSize(24);
    m_target_hp_text.setOutlineColor(sf::Color::Black);
    m_target_hp_text.setOutlineThickness(1.0f);
    m_target_hp_text.setOrigin({m_target_hp_text.getLocalBounds().width, 0.0f});

    updateTargetStats();

    updatePlayerSkills();
}

void LevelGui::resize(const sf::Vector2f &size)
{
    m_size = size;

    // player
    m_player_stats_bg_sprite.setPosition(m_border_offset);
    m_player_hp_bar_sprite.setPosition(
        m_border_offset + vector2::mult(sf::Vector2f{2.0f, 2.0f}, m_stats_scale));
    m_player_armor_bar_sprite.setPosition(
        m_border_offset + vector2::mult(sf::Vector2f{2.0f, 10.0f}, m_stats_scale));
    m_player_hp_text.setPosition(
        m_border_offset + vector2::mult(sf::Vector2f{4.0f, -2.0f}, m_stats_scale));
    m_player_armor_text.setPosition(
        m_border_offset + vector2::mult(sf::Vector2f{4.0f, 6.0f}, m_stats_scale));

    // skills
    if (!m_skills.empty())
    {
        sf::Vector2f skill_icon_size = m_skills.front().sprite.getLocalBounds().getSize();

        for (int32_t i = 0; i < m_skills.size(); ++i)
        {
            float space = i == 0 ? 0.0f : 4.0f * m_stats_scale.x;
            m_skills[m_skills.size() - 1 - i].sprite.setPosition(
                m_size - m_border_offset - sf::Vector2f{skill_icon_size.x + space, 0} * float(i));
            m_skills[m_skills.size() - 1 - i].key_text.setPosition(
                m_size - m_border_offset
                - sf::Vector2f{(skill_icon_size.x + space) * float(i), 4.0f});
        }
    }

    // target
    m_target_stats_bg_sprite.setPosition({m_size.x - m_border_offset.x, m_border_offset.y});
    m_target_hp_bar_sprite.setPosition(
        sf::Vector2f{m_size.x - m_border_offset.x, m_border_offset.y}
        + vector2::mult(sf::Vector2f{-6.0f, 2.0f}, m_stats_scale));
    m_target_hp_text.setPosition(
        sf::Vector2f{m_size.x - m_border_offset.x, m_border_offset.y}
        + vector2::mult(sf::Vector2f{-8.0f, -2.0f}, m_stats_scale));
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

    sf::IntRect hp_bar_rect = m_player_hp_bar_sprite.getTextureRect();
    hp_bar_rect.width = m_stats_hp_bar_width * health_ratio;
    m_player_hp_bar_sprite.setTextureRect(hp_bar_rect);

    sf::IntRect armor_bar_rect = m_player_armor_bar_sprite.getTextureRect();
    armor_bar_rect.width = m_stats_armor_bar_width * armor_ratio;
    m_player_armor_bar_sprite.setTextureRect(armor_bar_rect);

    m_player_hp_text.setString(hp_text);
    m_player_armor_text.setString(armor_text);
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
                {s.key_text.getLocalBounds().width / 2 + s.sprite.getLocalBounds().width / 2,
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

    sf::IntRect hp_bar_rect = m_target_hp_bar_sprite.getTextureRect();
    hp_bar_rect.width = m_stats_hp_bar_width * health_ratio;
    m_target_hp_bar_sprite.setTextureRect(hp_bar_rect);
    m_target_hp_bar_sprite.setOrigin({m_target_hp_bar_sprite.getLocalBounds().width, 0.0f});

    m_target_hp_text.setString(hp_text);
    m_target_hp_text.setOrigin({m_target_hp_text.getLocalBounds().width, 0.0f});
}

void LevelGui::drawPlayerStats(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_player_stats_bg_sprite);
    target.draw(m_player_hp_bar_sprite);
    target.draw(m_player_armor_bar_sprite);
    target.draw(m_player_hp_text);
    target.draw(m_player_armor_text);
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
    target.draw(m_target_stats_bg_sprite);
    target.draw(m_target_hp_bar_sprite);
    target.draw(m_target_hp_text);
}

} // namespace fck
