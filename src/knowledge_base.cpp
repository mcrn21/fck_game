#include "knowledge_base.h"

#include "entity_utils.h"

#include "components/components.h"

#include <filesystem>

namespace fck
{

std::pair<Drawable *, DrawableAnimation *> KnowledgeBase::createDrawable(
    const std::string &drawable_name)
{
    auto drawables_found = instance().m_drawables.find(drawable_name);
    if (drawables_found == instance().m_drawables.end())
        return {nullptr, nullptr};
    return drawables_found->second->create();
}

void KnowledgeBase::loadDrawablesDirectory(const std::string &dir_name)
{
    spdlog::info("Load drawable directory...");

    for (const auto &entry : std::filesystem::directory_iterator(dir_name))
    {
        std::string file_name = entry.path().relative_path().string();

        toml::table table;
        try
        {
            table = toml::parse_file(file_name);

            std::string sprite_name = table.at("name").as_string()->get();
            drawable_type::Type type
                = drawable_type::fromString(table.at("type").as_string()->get());
            if (type == drawable_type::NO_TYPE)
                throw Exception{"Wrong drawable type"};

            std::unique_ptr<DrawableItemBase> drawable_item;
            drawable_item.reset(instance().m_drawable_fabrics[type]());
            if (drawable_item)
            {
                drawable_item->init(&table);
                instance().m_drawables.emplace(sprite_name, std::move(drawable_item));
                spdlog::info("Add drawable: {}", sprite_name);
            }
        }
        catch (const std::exception &e)
        {
            spdlog::warn("Error parsing sprite file \"{}\": {}", file_name, e.what());
        }
    }
}

Entity KnowledgeBase::createEntity(const std::string &entity_name, World *world)
{
    if (!world)
        return {};

    auto entities_found = instance().m_entities.find(entity_name);
    if (entities_found == instance().m_entities.end())
        return Entity{};

    Entity entity = world->createEntity();

    for (const auto &it : entities_found->second->components)
        it->create(entity);

    // notify
    if (entity.hasComponent<TransformComponent>())
        EntityUtils::entity_moved.emit(entity, sf::Vector2f{});

    if (entity.hasComponent<StateComponent>())
    {
        EntityUtils::entity_state_changed.emit(entity, entity_state::IDLE);
        EntityUtils::entity_direction_changed.emit(entity, entity_state::RIGHT);
    }

    return entity;
}

Entity KnowledgeBase::createPlayer(const std::string &entity_name, World *world)
{
    Entity entity = createEntity(entity_name, world);

    PlayerComponent &player_component = entity.addComponent<PlayerComponent>();

    TypeComponent &type_component = entity.addComponent<TypeComponent>();
    type_component.type = entity_type::PLAYER;

    return entity;
}

void KnowledgeBase::loadEntitiesDirectory(const std::string &dir_name)
{
    spdlog::info("Load entity directory...");

    for (const auto &entry : std::filesystem::directory_iterator(dir_name))
    {
        std::string file_name = entry.path().relative_path().string();

        toml::table table;
        try
        {
            table = toml::parse_file(file_name);

            std::unique_ptr<EntityItem> entity_item = std::make_unique<EntityItem>();

            entity_item->name = table.at("name").as_string()->get();

            if (table.contains("components") && table.at("components").is_table())
            {
                toml::table *components_table = table.at("components").as_table();

                for (auto &it : *components_table)
                {
                    if (!it.second.is_table())
                        continue;

                    toml::table *component_table = it.second.as_table();

                    component_type::Type component_type
                        = component_type::fromString(it.first.data());

                    auto component_fabrics_found
                        = instance().m_component_fabrics.find(component_type);
                    if (component_fabrics_found == instance().m_component_fabrics.end())
                        throw Exception{fmt::format("Component not found: {}", it.first.data())};

                    ComponentItemBase *component_item = component_fabrics_found->second();
                    if (!component_item)
                        throw Exception{fmt::format("Component error")};

                    component_item->init(component_table);

                    entity_item->components.push_back(
                        std::unique_ptr<ComponentItemBase>(component_item));
                }
            }

            spdlog::info("Add entity: {}", entity_item->name);
            instance().m_entities.emplace(entity_item->name, std::move(entity_item));
        }
        catch (const std::exception &e)
        {
            spdlog::warn("Error parsing entity file \"{}\": {}", file_name, e.what());
        }
    }
}

KnowledgeBase::SkillItemBase *KnowledgeBase::skill(const std::string &name)
{
    auto skills_found = instance().m_skills.find(name);
    if (skills_found == instance().m_skills.end())
        return nullptr;
    return skills_found->second.get();
}

void KnowledgeBase::loadSkillsDirectory(const std::string &dir_name)
{
    spdlog::info("Load skills directory...");

    for (const auto &entry : std::filesystem::directory_iterator(dir_name))
    {
        std::string file_name = entry.path().relative_path().string();

        toml::table table;
        try
        {
            table = toml::parse_file(file_name);

            std::string base_skill = table.at("base_skill").as_string()->get();

            auto skill_fabrics = instance().m_base_skill_fabrics.find(base_skill);
            if (skill_fabrics == instance().m_base_skill_fabrics.end())
                throw Exception{fmt::format("Base skill not found: {}", base_skill)};

            std::unique_ptr<SkillItemBase> skill_item_base;
            skill_item_base.reset(skill_fabrics->second());

            skill_item_base->init(&table);

            spdlog::info("Add skill: {}", skill_item_base->name());
            instance().m_skills.emplace(skill_item_base->name(), std::move(skill_item_base));
        }
        catch (const std::exception &e)
        {
            spdlog::warn("Error parsing sprite file \"{}\": {}", file_name, e.what());
        }
    }
}

EntityScriptBase *KnowledgeBase::createEntityScript(const std::string &name)
{
    auto entity_scripts_found = instance().m_entity_scripts.find(name);
    if (entity_scripts_found == instance().m_entity_scripts.end())
        return nullptr;
    return entity_scripts_found->second->create();
}

void KnowledgeBase::loadEntityScriptsDirectory(const std::string &dir_name)
{
    spdlog::info("Load entity scripts directory...");

    for (const auto &entry : std::filesystem::directory_iterator(dir_name))
    {
        std::string file_name = entry.path().relative_path().string();

        toml::table table;
        try
        {
            table = toml::parse_file(file_name);

            std::string base_skill = table.at("base_entity_script").as_string()->get();

            auto base_entity_script_fabrics_found
                = instance().m_base_entity_script_fabrics.find(base_skill);
            if (base_entity_script_fabrics_found == instance().m_base_entity_script_fabrics.end())
                throw Exception{fmt::format("Base entity script not found: {}", base_skill)};

            std::unique_ptr<EntityScriptItemBase> entity_script_item_base;
            entity_script_item_base.reset(base_entity_script_fabrics_found->second());

            entity_script_item_base->init(&table);

            spdlog::info("Add entity script: {}", entity_script_item_base->name());
            instance().m_entity_scripts.emplace(
                entity_script_item_base->name(), std::move(entity_script_item_base));
        }
        catch (const std::exception &e)
        {
            spdlog::warn("Error parsing sprite file \"{}\": {}", file_name, e.what());
        }
    }
}

KnowledgeBase &KnowledgeBase::instance()
{
    static KnowledgeBase knowledge_base;
    return knowledge_base;
}

KnowledgeBase::KnowledgeBase()
{
}

} // namespace fck
