#ifndef KNOWLEDGEBASE_TFUTPCIFHOBA_H
#define KNOWLEDGEBASE_TFUTPCIFHOBA_H

#include "../fck/common.h"
#include "../fck/entity.h"
#include "../fck/resource_cache.h"
#include "../fck/utilities.h"
#include "../fck_common.h"
#include "../skills/skill_base.h"
#include "component_factory.h"
#include "entity_factory.h"
#include "script_factory.h"
#include "skill_factory.h"

#include "../sqlite/sqlite3.h"

#include <spdlog/spdlog.h>
#include <toml++/toml.h>

#include <typeindex>
#include <unordered_map>

#define KNOWLEDGE_BASE_REGISTER_BASE_SKILL(_class_, _base_skill_name_) \
    inline const bool knowledge_base_base_skill_##_class_ \
        = ::fck::KnowledgeBase::registerBaseSkill<_class_>(_base_skill_name_)

namespace fck
{

class KnowledgeBase
{
    friend class ComponentItemBase;

public:
    static void loadEntitiesDirectory(const std::string &dir_name);
    static void loadEntitiesFromDatabase(const std::string &database_name);

    // Skills
    struct SkillItemBase
    {
        friend class KnowledgeBase;

        virtual const std::string &getName() const = 0;
        virtual const std::string &getDisplayName() const = 0;
        virtual const std::string &getDisplayDescription() const = 0;
        virtual const std::string &getTextureName() const = 0;
        virtual const sf::IntRect &getTextureRect() const = 0;
        virtual SkillBase *create() const = 0;

    private:
        virtual void init(const std::string &name, toml::table *table) = 0;
    };

    template<typename T>
    struct SkillItem : SkillItemBase
    {
    };

    static SkillItemBase *getSkill(const std::string &name);

    template<typename T>
    static bool registerBaseSkill(const std::string &base_skill_name);

    static void loadSkillsDirectory(const std::string &dir_name);
    static void loadSkillsFromDatabase(const std::string &database_name);

    // Scripts
    static void loadScriptsFromDatabase(const std::string &database_name);

private:
    static KnowledgeBase &instance();

    static int32_t loadEntitiesFromDatabaseCallback(
        void *user_data, int argc, char **argv, char **column_name);
    static int32_t loadSkillsFromDatabaseCallback(
        void *user_data, int argc, char **argv, char **column_name);

    // Scripts
    static int32_t loadScriptsFromDatabaseCallback(
        void *user_data, int argc, char **argv, char **column_name);

    KnowledgeBase() = default;
    ~KnowledgeBase() = default;

    void loadEntityFromBuffer(const std::string &name, const std::string &data);
    void loadSkillFromBuffer(const std::string &name, const std::string &data);
    void loadEntityScriptFromBuffer(const std::string &name, const std::string &data);

    // Scripts
    void loadScriptFromBuffer(const std::string &name, const std::string &data);

private:
    std::unordered_map<std::string, std::function<SkillItemBase *()>> m_base_skill_fabrics;
    std::unordered_map<std::string, std::unique_ptr<SkillItemBase>> m_skills;
};

template<typename T>
bool KnowledgeBase::registerBaseSkill(const std::string &base_skill_name)
{
    spdlog::info("Register base skill: {}", base_skill_name);
    instance().m_base_skill_fabrics.emplace(base_skill_name, []() { return new SkillItem<T>(); });
    return true;
}

} // namespace fck

#endif // KNOWLEDGEBASE_TFUTPCIFHOBA_H
