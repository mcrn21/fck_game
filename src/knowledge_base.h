#ifndef KNOWLEDGEBASE_TFUTPCIFHOBA_H
#define KNOWLEDGEBASE_TFUTPCIFHOBA_H

#include "entity_scripts/entity_script_base.h"
#include "fck_common.h"
#include "skills/skill_base.h"

#include "fck/common.h"
#include "fck/drawable_animation.h"
#include "fck/drawable_proxy.h"
#include "fck/drawable_state.h"
#include "fck/entity.h"
#include "fck/resource_cache.h"
#include "fck/sprite_animation.h"
#include "fck/sprite_state.h"
#include "fck/utilities.h"

#include "sqlite/sqlite3.h"

#include <SFML/Graphics.hpp>

#include <toml++/toml.h>

#include <spdlog/spdlog.h>

#include <tuple>
#include <typeindex>
#include <unordered_map>

#define KNOWLEDGE_BASE_REGISTER_DRAWABLE(_class1_, _class2_, _class3_) \
    inline const bool knowledge_base_drawable_##_class1_ \
        = ::fck::KnowledgeBase::registerDrawable<_class1_, _class2_, _class3_>()

#define KNOWLEDGE_BASE_REGISTER_COMPONENT(_class_) \
    inline const bool knowledge_base_component_##_class_ \
        = ::fck::KnowledgeBase::registerComponent<_class_>()

#define KNOWLEDGE_BASE_REGISTER_BASE_SKILL(_class_, _base_skill_name_) \
    inline const bool knowledge_base_base_skill_##_class_ \
        = ::fck::KnowledgeBase::registerBaseSkill<_class_>(_base_skill_name_)

#define KNOWLEDGE_BASE_REGISTER_BASE_ENTITY_SCRIPT(_class_, _base_entity_script_name_) \
    inline const bool knowledge_base_base_entity_script_##_class_ \
        = ::fck::KnowledgeBase::registerBaseEntityScript<_class_>(_base_entity_script_name_)

namespace fck
{

class KnowledgeBase
{
    friend class ComponentItemBase;

public:
    // Drawables
    struct DrawableItemBase
    {
        virtual void init(toml::table *table) = 0;
        virtual std::tuple<DrawableProxyBase *, DrawableState *, DrawableAnimation *> create() = 0;
    };

    template<typename T, typename U, typename V>
    struct DrawableItem : DrawableItemBase
    {
        static drawable_type::Type drawableType()
        {
            return drawable_type::NO_TYPE;
        }
    };

    template<typename T, typename U, typename V>
    static bool registerDrawable();

    // Entities
    struct ComponentItemBase
    {
        virtual void init(toml::table *table)
        {
        }
        virtual void create(Entity &entity) = 0;
    };

    template<typename T>
    struct ComponentItem : ComponentItemBase
    {
        static component_type::Type componentType()
        {
            return component_type::NO_TYPE;
        }
    };

    struct EntityItem
    {
        std::vector<std::unique_ptr<ComponentItemBase>> components;
    };

    static Entity createEntity(const std::string &entity_name, World *world);
    static Entity createPlayer(const std::string &entity_name, World *world);

    template<typename T>
    static bool registerComponent();

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

    // Entity scripts
    struct EntityScriptItemBase
    {
        friend class KnowledgeBase;

        virtual const std::string &getName() const = 0;
        virtual EntityScriptBase *create() const = 0;

    private:
        virtual void init(const std::string &name, toml::table *table) = 0;
    };

    template<typename T>
    struct EntityScriptItem : EntityScriptItemBase
    {
    };

    static EntityScriptBase *createEntityScript(const std::string &name);

    template<typename T>
    static bool registerBaseEntityScript(const std::string &base_entity_script_name);

    static void loadEntityScriptsDirectory(const std::string &dir_name);
    static void loadEntityScriptsFromDatabase(const std::string &database_name);

private:
    static KnowledgeBase &instance();

    static int32_t loadEntitiesFromDatabaseCallback(
        void *user_data, int argc, char **argv, char **column_name);
    static int32_t loadSkillsFromDatabaseCallback(
        void *user_data, int argc, char **argv, char **column_name);
    static int32_t loadEntityScriptsFromDatabaseCallback(
        void *user_data, int argc, char **argv, char **column_name);

    KnowledgeBase();
    ~KnowledgeBase() = default;

    DrawableItemBase *loadDrawableFromTable(const std::string &type_string, toml::table *table);
    void loadEntityFromBuffer(const std::string &name, const std::string &data);
    void loadSkillFromBuffer(const std::string &name, const std::string &data);
    void loadEntityScriptFromBuffer(const std::string &name, const std::string &data);

private:
    std::unordered_map<drawable_type::Type, std::function<DrawableItemBase *()>> m_drawable_fabrics;
    std::unordered_map<std::string, std::unique_ptr<DrawableItemBase>> m_drawables;

    std::unordered_map<component_type::Type, std::function<ComponentItemBase *()>>
        m_component_fabrics;
    std::unordered_map<std::string, std::unique_ptr<EntityItem>> m_entities;

    std::unordered_map<std::string, std::function<SkillItemBase *()>> m_base_skill_fabrics;
    std::unordered_map<std::string, std::unique_ptr<SkillItemBase>> m_skills;

    std::unordered_map<std::string, std::function<EntityScriptItemBase *()>>
        m_base_entity_script_fabrics;
    std::unordered_map<std::string, std::unique_ptr<EntityScriptItemBase>> m_entity_scripts;
};

template<typename T, typename U, typename V>
bool KnowledgeBase::registerDrawable()
{
    spdlog::info(
        "Register drawable: {}", drawable_type::toString(DrawableItem<T, U, V>::drawableType()));
    instance().m_drawable_fabrics.emplace(
        DrawableItem<T, U, V>::drawableType(), []() { return new DrawableItem<T, U, V>; });
    return true;
}

template<typename T>
bool KnowledgeBase::registerComponent()
{
    spdlog::info(
        "Register component: {}", component_type::toString(ComponentItem<T>::componentType()));
    instance().m_component_fabrics.emplace(
        ComponentItem<T>::componentType(), []() { return new ComponentItem<T>(); });
    return true;
}

template<typename T>
bool KnowledgeBase::registerBaseSkill(const std::string &base_skill_name)
{
    spdlog::info("Register base skill: {}", base_skill_name);
    instance().m_base_skill_fabrics.emplace(base_skill_name, []() { return new SkillItem<T>(); });
    return true;
}

template<typename T>
bool KnowledgeBase::registerBaseEntityScript(const std::string &base_entity_script_name)
{
    spdlog::info("Register base entity script: {}", base_entity_script_name);
    instance().m_base_entity_script_fabrics.emplace(
        base_entity_script_name, []() { return new EntityScriptItem<T>(); });
    return true;
}

// Drawables
template<>
struct KnowledgeBase::DrawableItem<sf::Sprite, SpriteState, SpriteAnimation> : DrawableItemBase
{
    static drawable_type::Type drawableType()
    {
        return drawable_type::SPRITE;
    }

    void init(toml::table *table)
    {
        std::string texture_name = table->at("texture").as_string()->get();
        texture = ResourceCache::get<sf::Texture>(texture_name);
        if (!texture)
            throw Exception{fmt::format("Texture not found: {}", texture_name)};

        if (table->contains("texture_rect"))
            texture_rect = rect::tomlArrayToIntRect(table->at("texture_rect").as_array());

        if (table->contains("states") && table->at("states").is_table())
        {
            toml::table *states_table = table->at("states").as_table();

            for (auto &it : *states_table)
            {
                if (!it.second.is_table())
                    continue;

                toml::table *state_table = it.second.as_table();

                sf::IntRect state_rect
                    = rect::tomlArrayToIntRect(state_table->at("texture_rect").as_array());

                states.emplace(it.first.data(), state_rect);
            }
        }

        if (table->contains("animations") && table->at("animations").is_table())
        {
            toml::table *abimations_table = table->at("animations").as_table();

            for (auto &it : *abimations_table)
            {
                if (!it.second.is_table())
                    continue;

                toml::table *state_table = it.second.as_table();

                int32_t interval = state_table->at("interval").as_integer()->get();
                bool repeat = state_table->at("repeat").as_boolean()->get();
                sf::IntRect frame_rect
                    = rect::tomlArrayToIntRect(state_table->at("texture_rect").as_array());
                sf::Vector2i frame_count
                    = vector2::tomlArrayToVector2i(state_table->at("frame_count").as_array());

                animations.emplace(
                    it.first.data(), Animation{interval, repeat, frame_rect, frame_count});
            }
        }

        if (!animations.empty())
            states.clear();
    }

    std::tuple<DrawableProxyBase *, DrawableState *, DrawableAnimation *> create()
    {

        sf::Sprite *sprite = new sf::Sprite{*texture, texture_rect};

        SpriteState *sprite_state = nullptr;
        if (!states.empty())
        {
            sprite_state = new SpriteState{*sprite};
            for (const auto &it : states)
                sprite_state->addState(it.first, it.second);
        }

        SpriteAnimation *sprite_animation = nullptr;
        if (!animations.empty())
        {
            sprite_animation = new SpriteAnimation{*sprite};
            for (const auto &it : animations)
            {
                sprite_animation->addState(
                    it.first,
                    sf::milliseconds(it.second.interval),
                    it.second.repeat,
                    it.second.texture_rect,
                    it.second.frame_count);
            }
        }

        return {new DrawableProxy(sprite), sprite_state, sprite_animation};
    }

    struct State
    {
        sf::IntRect texture_rect;
    };

    struct Animation
    {
        int32_t interval = 0;
        bool repeat = false;
        sf::IntRect texture_rect;
        sf::Vector2i frame_count;
    };

    sf::Texture *texture = nullptr;
    sf::IntRect texture_rect;
    std::unordered_map<std::string, sf::IntRect> states;
    std::unordered_map<std::string, Animation> animations;
};

inline const bool knowledge_base_drawable_sprite
    = ::fck::KnowledgeBase::registerDrawable<sf::Sprite, SpriteState, SpriteAnimation>();

//KNOWLEDGE_BASE_REGISTER_DRAWABLE(sf::Sprite, SpriteState, SpriteAnimation);

} // namespace fck

#endif // KNOWLEDGEBASE_TFUTPCIFHOBA_H
