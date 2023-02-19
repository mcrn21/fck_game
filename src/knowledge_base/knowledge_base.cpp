#include "knowledge_base.h"
#include "../components/components.h"
#include "../fck/world.h"

#include <filesystem>
#include <fstream>

namespace fck
{

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

void KnowledgeBase::loadScriptsFromDatabase(const std::string &database_name)
{
    sqlite3 *db = nullptr;

    int32_t rc = sqlite3_open(database_name.c_str(), &db);

    if (rc)
    {
        spdlog::error("Can't open database: {}", sqlite3_errmsg(db));
        return;
    }

    spdlog::info("Load scripts from database");

    std::string sql = "SELECT * from scripts";

    char *error_message = nullptr;
    rc = sqlite3_exec(
        db,
        sql.c_str(),
        &KnowledgeBase::loadScriptsFromDatabaseCallback,
        &instance(),
        &error_message);

    if (rc)
        spdlog::warn("Can't load scripts from database: {}", error_message);

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

int32_t KnowledgeBase::loadScriptsFromDatabaseCallback(
    void *user_data, int argc, char **argv, char **column_name)
{
    try
    {
        reinterpret_cast<KnowledgeBase *>(user_data)->loadScriptFromBuffer(
            std::string(argv[0]), std::string(argv[1]));
    }
    catch (const std::exception &e)
    {
        spdlog::warn("Error parsing script properties \"{}\": {}", argv[0], e.what());
    }

    return 0;
}

void KnowledgeBase::loadEntityFromBuffer(const std::string &name, const std::string &data)
{
    EntityFactory::registerEntityFactory(name, data);
}

void KnowledgeBase::loadSkillFromBuffer(const std::string &name, const std::string &data)
{
    SkillFactory::registerSkillFactory(name, data);

    //    toml::table table = toml::parse(data);

    //    std::string base_skill = table.at("base_skill").as_string()->get();

    //    auto skill_fabrics = m_base_skill_fabrics.find(base_skill);
    //    if (skill_fabrics == m_base_skill_fabrics.end())
    //        throw Exception{fmt::format("Base skill not found: {}", base_skill)};

    //    std::unique_ptr<SkillItemBase> skill_item_base;
    //    skill_item_base.reset(skill_fabrics->second());

    //    skill_item_base->init(name, &table);

    //    spdlog::info("Add skill: {}", name);
    //    m_skills.emplace(name, std::move(skill_item_base));
}

void KnowledgeBase::loadScriptFromBuffer(const std::string &name, const std::string &data)
{
    //    spdlog::debug("Load lua script: {}: {}", name, data);
    ScriptFactory::registerScriptFactory(name, data);
}

} // namespace fck
