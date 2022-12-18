#include "knowledge_base.h"

#include "entity_utils.h"

#include "components/components.h"

#include <filesystem>
#include <fstream>

namespace fck
{

Entity KnowledgeBase::createEntity(const std::string &entity_name, World *world)
{
    if (!world)
        return {};

    auto entities_found = instance().m_entities.find(entity_name);
    if (entities_found == instance().m_entities.end())
    {
        spdlog::warn("Entity not found: {}", entity_name);
        return Entity{};
    }

    Entity entity = world->createEntity();

    for (const auto &it : entities_found->second->components)
        it->create(entity);

    return entity;
}

Entity KnowledgeBase::createPlayer(const std::string &entity_name, World *world)
{
    Entity entity = createEntity(entity_name, world);

    component::Player &player_component = entity.addComponent<component::Player>();

    component::Type &type_component = entity.addComponent<component::Type>();
    type_component.type = entity_type::PLAYER;

    return entity;
}

void KnowledgeBase::loadEntitiesDirectory(const std::string &dir_name)
{
    spdlog::info("Load entity directory...");

    for (const auto &entry : std::filesystem::recursive_directory_iterator(dir_name))
    {
        if (entry.is_directory())
            continue;

        std::string file_name = entry.path().relative_path().string();
        std::string entity_name = entry.path().filename().string();

        try
        {
            std::ifstream ifs(file_name, std::ios_base::in);
            instance().loadEntityFromBuffer(
                entity_name, std::string(std::istreambuf_iterator<char>{ifs}, {}));
        }
        catch (const std::exception &e)
        {
            spdlog::warn("Error parsing entity file \"{}\": {}", file_name, e.what());
        }
    }
}

void KnowledgeBase::loadEntitiesFromDatabase(const std::string &database_name)
{
    sqlite3 *db = nullptr;

    int32_t rc = sqlite3_open(database_name.c_str(), &db);

    if (rc)
    {
        spdlog::error("Can't open database: {}", sqlite3_errmsg(db));
        return;
    }

    spdlog::info("Load entities from database");

    std::string sql = "SELECT * from entities";

    char *error_message = nullptr;
    rc = sqlite3_exec(
        db,
        sql.c_str(),
        &KnowledgeBase::loadEntitiesFromDatabaseCallback,
        &instance(),
        &error_message);

    if (rc)
        spdlog::warn("Can't load entities from database: {}", error_message);

    sqlite3_close(db);
}

KnowledgeBase::SkillItemBase *KnowledgeBase::getSkill(const std::string &name)
{
    auto skills_found = instance().m_skills.find(name);
    if (skills_found == instance().m_skills.end())
        return nullptr;
    return skills_found->second.get();
}

void KnowledgeBase::loadSkillsDirectory(const std::string &dir_name)
{
    spdlog::info("Load skills directory...");

    for (const auto &entry : std::filesystem::recursive_directory_iterator(dir_name))
    {
        if (entry.is_directory())
            continue;

        std::string file_name = entry.path().relative_path().string();
        std::string skill_name = entry.path().filename().string();

        try
        {
            std::ifstream ifs(file_name, std::ios_base::in);
            instance().loadSkillFromBuffer(
                skill_name, std::string(std::istreambuf_iterator<char>{ifs}, {}));
        }
        catch (const std::exception &e)
        {
            spdlog::warn("Error parsing sprite file \"{}\": {}", file_name, e.what());
        }
    }
}

void KnowledgeBase::loadSkillsFromDatabase(const std::string &database_name)
{
    sqlite3 *db = nullptr;

    int32_t rc = sqlite3_open(database_name.c_str(), &db);

    if (rc)
    {
        spdlog::error("Can't open database: {}", sqlite3_errmsg(db));
        return;
    }

    spdlog::info("Load skills from database");

    std::string sql = "SELECT * from skills";

    char *error_message = nullptr;
    rc = sqlite3_exec(
        db,
        sql.c_str(),
        &KnowledgeBase::loadSkillsFromDatabaseCallback,
        &instance(),
        &error_message);

    if (rc)
        spdlog::warn("Can't load skills from database: {}", error_message);

    sqlite3_close(db);
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

    for (const auto &entry : std::filesystem::recursive_directory_iterator(dir_name))
    {
        if (entry.is_directory())
            continue;

        std::string file_name = entry.path().relative_path().string();
        std::string entity_script_name = entry.path().filename().string();

        toml::table table;
        try
        {
            std::ifstream ifs(file_name, std::ios_base::in);
            instance().loadEntityScriptFromBuffer(
                entity_script_name, std::string(std::istreambuf_iterator<char>{ifs}, {}));
        }
        catch (const std::exception &e)
        {
            spdlog::warn("Error parsing sprite file \"{}\": {}", file_name, e.what());
        }
    }
}

void KnowledgeBase::loadEntityScriptsFromDatabase(const std::string &database_name)
{
    sqlite3 *db = nullptr;

    int32_t rc = sqlite3_open(database_name.c_str(), &db);

    if (rc)
    {
        spdlog::error("Can't open database: {}", sqlite3_errmsg(db));
        return;
    }

    spdlog::info("Load entity scripts from database");

    std::string sql = "SELECT * from entity_scripts";

    char *error_message = nullptr;
    rc = sqlite3_exec(
        db,
        sql.c_str(),
        &KnowledgeBase::loadEntityScriptsFromDatabaseCallback,
        &instance(),
        &error_message);

    if (rc)
        spdlog::warn("Can't load entity scripts from database: {}", error_message);

    sqlite3_close(db);
}

KnowledgeBase &KnowledgeBase::instance()
{
    static KnowledgeBase knowledge_base;
    return knowledge_base;
}

int32_t KnowledgeBase::loadEntitiesFromDatabaseCallback(
    void *user_data, int argc, char **argv, char **column_name)
{
    try
    {
        reinterpret_cast<KnowledgeBase *>(user_data)->loadEntityFromBuffer(
            std::string(argv[0]), std::string(argv[1]));
    }
    catch (const std::exception &e)
    {
        spdlog::warn("Error parsing entity properties \"{}\": {}", argv[0], e.what());
    }

    return 0;
}

int32_t KnowledgeBase::loadSkillsFromDatabaseCallback(
    void *user_data, int argc, char **argv, char **column_name)
{
    try
    {
        reinterpret_cast<KnowledgeBase *>(user_data)->loadSkillFromBuffer(
            std::string(argv[0]), std::string(argv[1]));
    }
    catch (const std::exception &e)
    {
        spdlog::warn("Error parsing skill properties \"{}\": {}", argv[0], e.what());
    }

    return 0;
}

int32_t KnowledgeBase::loadEntityScriptsFromDatabaseCallback(
    void *user_data, int argc, char **argv, char **column_name)
{
    try
    {
        reinterpret_cast<KnowledgeBase *>(user_data)->loadEntityScriptFromBuffer(
            std::string(argv[0]), std::string(argv[1]));
    }
    catch (const std::exception &e)
    {
        spdlog::warn("Error parsing entity script properties \"{}\": {}", argv[0], e.what());
    }

    return 0;
}

KnowledgeBase::KnowledgeBase()
{
}

KnowledgeBase::DrawableItemBase *KnowledgeBase::loadDrawableFromTable(
    const std::string &type_string, toml::table *table)
{
    drawable_type::Type type = drawable_type::fromString(type_string);

    auto drawable_fabrics_found = m_drawable_fabrics.find(type);
    if (drawable_fabrics_found == m_drawable_fabrics.end())
        return nullptr;

    DrawableItemBase *drawable_item = m_drawable_fabrics[type]();

    if (drawable_item)
        drawable_item->init(table);

    return drawable_item;
}

void KnowledgeBase::loadEntityFromBuffer(const std::string &name, const std::string &data)
{
    toml::table table = toml::parse(data);

    std::unique_ptr<EntityItem> entity_item = std::make_unique<EntityItem>();

    if (table.contains("components") && table.at("components").is_table())
    {
        toml::table *components_table = table.at("components").as_table();

        for (auto &it : *components_table)
        {
            if (!it.second.is_table())
                continue;

            toml::table *component_table = it.second.as_table();

            component_type::Type component_type = component_type::fromString(it.first.data());

            auto component_fabrics_found = m_component_fabrics.find(component_type);
            if (component_fabrics_found == m_component_fabrics.end())
                throw Exception{fmt::format("Component not found: {}", it.first.data())};

            ComponentItemBase *component_item = component_fabrics_found->second();
            if (!component_item)
                throw Exception{fmt::format("Component error")};

            component_item->init(component_table);

            entity_item->components.push_back(std::unique_ptr<ComponentItemBase>(component_item));
        }
    }

    spdlog::info("Add entity: {}", name);
    m_entities.emplace(name, std::move(entity_item));
}

void KnowledgeBase::loadSkillFromBuffer(const std::string &name, const std::string &data)
{
    toml::table table = toml::parse(data);

    std::string base_skill = table.at("base_skill").as_string()->get();

    auto skill_fabrics = m_base_skill_fabrics.find(base_skill);
    if (skill_fabrics == m_base_skill_fabrics.end())
        throw Exception{fmt::format("Base skill not found: {}", base_skill)};

    std::unique_ptr<SkillItemBase> skill_item_base;
    skill_item_base.reset(skill_fabrics->second());

    skill_item_base->init(name, &table);

    spdlog::info("Add skill: {}", name);
    m_skills.emplace(name, std::move(skill_item_base));
}

void KnowledgeBase::loadEntityScriptFromBuffer(const std::string &name, const std::string &data)
{
    toml::table table = toml::parse(data);

    std::string base_skill = table.at("base_entity_script").as_string()->get();

    auto base_entity_script_fabrics_found = m_base_entity_script_fabrics.find(base_skill);
    if (base_entity_script_fabrics_found == m_base_entity_script_fabrics.end())
        throw Exception{fmt::format("Base entity script not found: {}", base_skill)};

    std::unique_ptr<EntityScriptItemBase> entity_script_item_base;
    entity_script_item_base.reset(base_entity_script_fabrics_found->second());

    entity_script_item_base->init(name, &table);

    spdlog::info("Add entity script: {}", name);
    m_entity_scripts.emplace(name, std::move(entity_script_item_base));
}

} // namespace fck
