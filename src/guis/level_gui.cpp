#include "level_gui.h"

#include "../components/components.h"

#include "../fck/resource_cache.h"

namespace fck
{

LevelGui::LevelGui(sf::View *view, const Entity &player_entity)
    : m_view{view}, m_player_entity(player_entity)
{
    m_scale = {1.0f, 1.0f};
    m_border_offset = {20.0f, 40.0f};

    m_player_stats_window_size = {284.0f * m_scale.x, 64.0f * m_scale.y};
    m_target_stats_window_size = {284.0f * m_scale.x, 128.0f * m_scale.y};

    // player
    m_player_stats_bg_sprite.setTexture(*ResourceCache::resource<sf::Texture>("hud"));
    m_player_stats_bg_sprite.setTextureRect(sf::IntRect{{0, 0}, {284, 64}});
    m_player_stats_bg_sprite.scale(m_scale);

    // target
    m_target_stats_bg_sprite.setTexture(*ResourceCache::resource<sf::Texture>("hud"));
    m_target_stats_bg_sprite.setTextureRect(sf::IntRect{{0, 64}, {284, 32}});
    m_target_stats_bg_sprite.scale(m_scale);

    m_target_skull_sprite.setTexture(*ResourceCache::resource<sf::Texture>("hud"));
    m_target_skull_sprite.setTextureRect(sf::IntRect{{0, 136}, {44, 48}});
    m_target_skull_sprite.scale(m_scale);

    // common
    m_stats_hp_bar_sprite.setTexture(*ResourceCache::resource<sf::Texture>("hud"));
    m_stats_hp_bar_sprite.setTextureRect(sf::IntRect{{8, 100}, {268, 12}});
    m_stats_hp_bar_sprite.scale(m_scale);
    m_stats_hp_bar_width = 268;

    m_stats_armor_bar_sprite.setTexture(*ResourceCache::resource<sf::Texture>("hud"));
    m_stats_armor_bar_sprite.setTextureRect(sf::IntRect{{8, 120}, {200, 12}});
    m_stats_armor_bar_sprite.scale(m_scale);
    m_stats_armor_bar_width = 200;

    // player skills
    SkillsComponent &skills_component = m_player_entity.component<SkillsComponent>();
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
            m_skills.push_back(s);
        }
    }
}

void LevelGui::draw()
{
    if (!m_player_entity.isValid())
        return;

    drawPlayerStats(m_player_entity);
    drawPlayerSkills(m_player_entity);

    TargetComponent &target_component = m_player_entity.component<TargetComponent>();
    if (!target_component.target.isValid())
        return;
    drawTargetStats(target_component.target);
}

void LevelGui::drawPlayerStats(const Entity &entity)
{
    StatsComponent &stats_component = entity.component<StatsComponent>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));
    std::string armor_text = std::to_string(int32_t(stats_component.armor)) + "/"
        + std::to_string(int32_t(stats_component.max_armor));

    float health_ratio = stats_component.health / stats_component.max_health;
    float armor_ratio = stats_component.armor / stats_component.max_armor;

    ImGui::SetNextWindowPos(m_border_offset);
    ImGui::SetNextWindowSize(m_player_stats_window_size);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});

    ImGui::PushFont(GuiBase::hp_hud_font);

    ImGui::Begin(
        "Player stats", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    // content
    ImGui::Image(m_player_stats_bg_sprite);

    ImGui::SetCursorPos({8.0f * m_scale.x, 8.0f * m_scale.y});
    sf::IntRect player_hp_bar_rect = m_stats_hp_bar_sprite.getTextureRect();
    player_hp_bar_rect.width = m_stats_hp_bar_width * health_ratio;
    m_stats_hp_bar_sprite.setTextureRect(player_hp_bar_rect);
    ImGui::Image(m_stats_hp_bar_sprite);

    ImGui::SetCursorPos({8.0f * m_scale.x, 40.0f * m_scale.y});
    sf::IntRect player_armor_bar_rect = m_stats_armor_bar_sprite.getTextureRect();
    player_armor_bar_rect.width = m_stats_armor_bar_width * armor_ratio;
    m_stats_armor_bar_sprite.setTextureRect(player_armor_bar_rect);
    ImGui::Image(m_stats_armor_bar_sprite);

    ImGui::SetCursorPos({14.0f * m_scale.x, -1.0f * m_scale.y});
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    ImGui::Text("%s", hp_text.c_str());
    ImGui::PopStyleColor(1);

    ImGui::SetCursorPos({12.0f * m_scale.x, -1.5f * m_scale.y});
    ImGui::Text("%s", hp_text.c_str());

    ImGui::SetCursorPos({14.0f * m_scale.x, 31.0f * m_scale.y});
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    ImGui::Text("%s", armor_text.c_str());
    ImGui::PopStyleColor(1);

    ImGui::SetCursorPos({12.0f * m_scale.x, 30.0f * m_scale.y});
    ImGui::Text("%s", armor_text.c_str());

    ImGui::End();

    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(1);
}

void LevelGui::drawPlayerSkills(const Entity &entity)
{
    if (m_skills.empty())
        return;

    sf::Vector2f view_size = m_view->getSize();
    sf::Vector2i skill_icon_size = m_skills.front().sprite.getTextureRect().getSize();
    float window_width = m_skills.size() * skill_icon_size.x * m_scale.x;

    ImGui::SetNextWindowPos(ImVec2{
        view_size.x - m_border_offset.x - window_width,
        view_size.y - m_border_offset.y - skill_icon_size.x * m_scale.y});
    ImGui::SetNextWindowSize({window_width, skill_icon_size.y * m_scale.x});

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});

    ImGui::PushFont(GuiBase::hp_hud_font);

    ImGui::Begin(
        "Player actions", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    std::vector<std::string> key_texts = {"J", "K", "L"};

    for (int32_t i = 0; i < m_skills.size(); ++i)
    {
        ImGui::SetCursorPos({float(skill_icon_size.x * m_scale.x * i), 0});

        ImGui::BeginDisabled(!m_skills[i].skill->isReady());
        ImGui::Image(m_skills[i].sprite);
        ImGui::EndDisabled();

        std::string key_text = key_texts[i];
        ImVec2 key_text_size = ImGui::CalcTextSize(key_text.c_str());

        ImGui::SetCursorPos(
            {float(skill_icon_size.x * m_scale.x * i) + (skill_icon_size.x - 11.0f) * m_scale.x
                 - key_text_size.x,
             13.0f * m_scale.y});

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
        ImGui::Text("%s", key_text.c_str());
        ImGui::PopStyleColor(1);

        ImGui::SetCursorPos(
            {float(skill_icon_size.x * m_scale.x * i) + (skill_icon_size.x - 12.0f) * m_scale.x
                 - key_text_size.x,
             12.0f * m_scale.y});
        ImGui::Text("%s", key_text.c_str());

        //        ImGui::SetCursorPos(ImVec2{float(10 + 60 * i), 70});
        //        ImGui::ProgressBar(1 - (action->remained() / action->cooldown()), ImVec2{50, 10}, "");
    }

    ImGui::End();

    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(1);
}

void LevelGui::drawTargetStats(const Entity &entity)
{
    StatsComponent &stats_component = entity.component<StatsComponent>();

    std::string hp_text = std::to_string(int32_t(stats_component.health)) + "/"
        + std::to_string(int32_t(stats_component.max_health));
    float health_ratio = stats_component.health / stats_component.max_health;

    sf::Vector2f view_size = m_view->getSize();

    ImGui::SetNextWindowPos(
        ImVec2{view_size.x - m_border_offset.x - m_target_stats_window_size.x, m_border_offset.y});
    ImGui::SetNextWindowSize(m_target_stats_window_size);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});

    ImGui::PushFont(GuiBase::hp_hud_font);

    ImGui::Begin(
        "Target stats", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    // content

    float bg_y_offset = 0.0f * m_scale.y;

    ImGui::SetCursorPos({0.0f, bg_y_offset});
    ImGui::Image(m_target_stats_bg_sprite);

    sf::IntRect hp_bar_rect = m_stats_hp_bar_sprite.getTextureRect();
    hp_bar_rect.width = m_stats_hp_bar_width * health_ratio;
    m_stats_hp_bar_sprite.setTextureRect(hp_bar_rect);

    ImGui::SetCursorPos(
        {m_target_stats_window_size.x - (hp_bar_rect.width + 8.0f) * m_scale.x,
         bg_y_offset + 8.0f * m_scale.y});
    ImGui::Image(m_stats_hp_bar_sprite);

    ImVec2 target_hp_text_size = ImGui::CalcTextSize(hp_text.c_str());

    ImGui::SetCursorPos(
        {m_target_stats_window_size.x - (target_hp_text_size.x + 10.0f * m_scale.x),
         bg_y_offset - 1.0f * m_scale.y});
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    ImGui::Text("%s", hp_text.c_str());
    ImGui::PopStyleColor(1);

    ImGui::SetCursorPos(
        {m_target_stats_window_size.x - (target_hp_text_size.x + 12.0f * m_scale.x),
         bg_y_offset - 1.5f * m_scale.y});
    ImGui::Text("%s", hp_text.c_str());

    ImGui::SetCursorPos(
        {m_target_stats_window_size.x - (m_target_skull_sprite.getTextureRect().width * m_scale.x),
         bg_y_offset + (m_target_stats_bg_sprite.getTextureRect().height) * m_scale.y});
    ImGui::Image(m_target_skull_sprite);

    ImGui::End();

    ImGui::PopFont();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(1);
}

} // namespace fck
