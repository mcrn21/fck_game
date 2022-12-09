#include "level_gui.h"

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

    m_minimap.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_minimap.setBackgroundTextureRect({{0, 40}, {32, 32}});
    m_minimap.setScale(m_stats_scale);

    m_minimap.setRoomTextureRects(
        {{0, {{1, 72}, {11, 11}}},
         {Level::Room::LEFT, {{1, 83}, {11, 11}}},
         {Level::Room::LEFT | Level::Room::TOP, {{12, 83}, {11, 11}}},
         {Level::Room::LEFT | Level::Room::TOP | Level::Room::RIGHT, {{23, 83}, {11, 11}}},
         {Level::Room::LEFT | Level::Room::TOP | Level::Room::RIGHT | Level::Room::BOTTOM,
          {{34, 83}, {11, 11}}},
         {Level::Room::TOP, {{1, 94}, {11, 11}}},
         {Level::Room::TOP | Level::Room::RIGHT, {{12, 94}, {11, 11}}},
         {Level::Room::TOP | Level::Room::RIGHT | Level::Room::BOTTOM, {{23, 94}, {11, 11}}},
         {Level::Room::RIGHT, {{1, 105}, {11, 11}}},
         {Level::Room::RIGHT | Level::Room::BOTTOM, {{12, 105}, {11, 11}}},
         {Level::Room::RIGHT | Level::Room::BOTTOM | Level::Room::LEFT, {{23, 105}, {11, 11}}},
         {Level::Room::BOTTOM, {{1, 116}, {11, 11}}},
         {Level::Room::BOTTOM | Level::Room::LEFT, {{12, 116}, {11, 11}}},
         {Level::Room::BOTTOM | Level::Room::LEFT | Level::Room::TOP, {{23, 116}, {11, 11}}},
         {Level::Room::TOP | Level::Room::BOTTOM, {{1, 127}, {11, 11}}},
         {Level::Room::LEFT | Level::Room::RIGHT, {{12, 127}, {11, 11}}}});

    m_minimap.setRoomUnopenedTextureRect({{12, 72}, {11, 11}});

    m_minimap.setRoomTypeTextureRects({{Level::Room::DEFAULT, {{0, 0}, {0, 0}}}});

    m_minimap.setCurrentRoomTextureRect({{1, 138}, {11, 11}});

    m_minimap.text().setFont(*ResourceCache::resource<sf::Font>("mini_pixel-7"));
    m_minimap.text().setCharacterSize(24);
    m_minimap.text().setOutlineColor(sf::Color::Black);
    m_minimap.text().setOutlineThickness(1.0f);
    m_minimap.text().setScale(vector2::div({1.0f, 1.0f}, m_stats_scale));
    m_minimap.text().setPosition({4.0f, m_minimap.localBounds().height - 7.0f});
    m_minimap.text().setString("M");

    m_test_frame.setTexture(*ResourceCache::resource<sf::Texture>("ui"));
    m_test_frame.setFrameTextureRect({{72, 0}, {40, 8}});
    m_test_frame.setFrameSize({40.0f, 40.0f});
    m_test_frame.setBorderSize({3, 3});
    m_test_frame.setScale(m_stats_scale);
    m_test_frame.setPosition({300.0f, 300.0f});

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

    m_minimap.setPosition(
        m_border_offset + vector2::mult(sf::Vector2f{0.0f, 16.0f}, m_stats_scale));

    //    sf::Vector2f minimap_sprite_global_center
    //        = m_minimap_sprite.getPosition() + m_minimap_sprite.globalBounds().getSize() / 2.0f;
    //    for (auto &room_sprite : m_room_minimap_sprites)
    //    {
    //        room_sprite.second.setPosition(
    //            minimap_sprite_global_center
    //            + vector2::mult(
    //                room_sprite.second.globalBounds().getSize(), sf::Vector2f{room_sprite.first}));
    //    }

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

    m_player_hp_progress_bar.setProgress(health_ratio);
    m_player_hp_progress_bar.text().setString(hp_text);

    m_player_armor_progress_bar.setProgress(armor_ratio);
    m_player_armor_progress_bar.text().setString(armor_text);
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
    m_target_hp_progress_bar.setProgress(health_ratio);

    m_target_hp_progress_bar.text().setString(hp_text);
    m_target_hp_progress_bar.text().setOrigin(
        {m_target_hp_progress_bar.text().getLocalBounds().width, 0.0f});
}

void LevelGui::updateRoomsMap(
    const Vector2D<Level::Room *> &rooms_map, const sf::Vector2i &current_room_coord)
{
    m_minimap.updateRooms(rooms_map);
    m_minimap.setCurrentRoom(current_room_coord);
}

void LevelGui::updateRoomOpened(
    const Vector2D<Level::Room *> &rooms_map, const sf::Vector2i &room_coord)
{
    m_minimap.setRoomOpened(rooms_map, room_coord);
}

void LevelGui::updateCurrentRoomCoord(const sf::Vector2i &current_room_coord)
{
    m_minimap.setCurrentRoom(current_room_coord);
}

void LevelGui::drawPlayerStats(sf::RenderTarget &target, const sf::RenderStates &states)
{
    target.draw(m_player_hp_progress_bar);
    target.draw(m_player_armor_progress_bar);

    target.draw(m_minimap);

    target.draw(m_test_frame);
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

} // namespace fck::gui
