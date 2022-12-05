#ifndef KNOWLEDGEBASE_H
#define KNOWLEDGEBASE_H

#include "entity_script_base.h"
#include "fck_common.h"
#include "skill_base.h"

#include "fck/common.h"
#include "fck/drawable.h"
#include "fck/drawable_animation.h"
#include "fck/entity.h"
#include "fck/resource_cache.h"
#include "fck/sprite.h"
#include "fck/sprite_animation.h"
#include "fck/utilities.h"

#include <toml++/toml.h>

#include <spdlog/spdlog.h>

#include <typeindex>
#include <unordered_map>

#define KNOWLEDGE_BASE_REGISTER_DRAWABLE(_class1_, _class2_) \
    inline const bool knowledge_base_drawable_##_class1_ \
        = ::fck::KnowledgeBase::registerDrawable<_class1_, _class2_>()

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
public:
    // Drawables
    struct DrawableItemBase
    {
        virtual void init(toml::table *table) = 0;
        virtual std::pair<Drawable *, DrawableAnimation *> create() = 0;
    };

    template<typename T, typename U>
    struct DrawableItem : DrawableItemBase
    {
        static drawable_type::Type drawableType()
        {
            return drawable_type::NO_TYPE;
        }
    };

    template<typename T, typename U>
    static std::pair<T *, U *> createDrawable(const std::string &drawable_name);
    static std::pair<Drawable *, DrawableAnimation *> createDrawable(
        const std::string &drawable_name);

    template<typename T, typename U>
    static bool registerDrawable();

    static void loadDrawablesDirectory(const std::string &dir_name);

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
        std::string name;
        std::vector<std::unique_ptr<ComponentItemBase>> components;
    };

    static Entity createEntity(const std::string &entity_name, World *world);
    static Entity createPlayer(const std::string &entity_name, World *world);

    template<typename T>
    static bool registerComponent();

    static void loadEntitiesDirectory(const std::string &dir_name);

    // Skills
    struct SkillItemBase
    {
        friend class KnowledgeBase;

        virtual const std::string &name() const = 0;
        virtual const std::string &displayName() const = 0;
        virtual const std::string &displayDescription() const = 0;
        virtual const std::string &textureName() const = 0;
        virtual const sf::IntRect &textureRect() const = 0;
        virtual SkillBase *create() const = 0;

    private:
        virtual void init(toml::table *table) = 0;
    };

    template<typename T>
    struct SkillItem : SkillItemBase
    {
    };

    static SkillItemBase *skill(const std::string &name);

    template<typename T>
    static bool registerBaseSkill(const std::string &base_skill_name);

    static void loadSkillsDirectory(const std::string &dir_name);

    // Entity scripts
    struct EntityScriptItemBase
    {
        friend class KnowledgeBase;

        virtual const std::string &name() const = 0;
        virtual EntityScriptBase *create() const = 0;

    private:
        virtual void init(toml::table *table) = 0;
    };

    template<typename T>
    struct EntityScriptItem : EntityScriptItemBase
    {
    };

    static EntityScriptBase *createEntityScript(const std::string &name);

    template<typename T>
    static bool registerBaseEntityScript(const std::string &base_entity_script_name);

    static void loadEntityScriptsDirectory(const std::string &dir_name);

private:
    static KnowledgeBase &instance();

    KnowledgeBase();
    ~KnowledgeBase() = default;

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

template<typename T, typename U>
std::pair<T *, U *> KnowledgeBase::createDrawable(const std::string &drawable_name)
{
    auto drawable = createDrawable(drawable_name);
    return {
        drawable.first ? static_cast<T *>(drawable.first) : nullptr,
        drawable.second ? static_cast<T *>(drawable.second) : nullptr};
}

template<typename T, typename U>
bool KnowledgeBase::registerDrawable()
{
    spdlog::info(
        "Register drawable: {}", drawable_type::toString(DrawableItem<T, U>::drawableType()));
    instance().m_drawable_fabrics.emplace(
        DrawableItem<T, U>::drawableType(), []() { return new DrawableItem<T, U>; });
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
struct KnowledgeBase::DrawableItem<Sprite, SpriteAnimation> : DrawableItemBase
{
    static drawable_type::Type drawableType()
    {
        return drawable_type::SPRITE;
    }

    void init(toml::table *table)
    {
        name = table->at("name").as_string()->get();

        std::string texture_name = table->at("texture").as_string()->get();
        texture = ResourceCache::resource<sf::Texture>(texture_name);
        if (!texture)
            throw Exception{fmt::format("Texture not found: {}", texture_name)};

        if (table->contains("texture_rect"))
            texture_rect = rect::tomlArrayToIntRect(table->at("texture_rect").as_array());

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
                    = rect::tomlArrayToIntRect(state_table->at("frame_rect").as_array());
                sf::Vector2i frame_count
                    = vector2::tomlArrayToVector2i(state_table->at("frame_count").as_array());

                animations.emplace(
                    it.first.data(), Animation{interval, repeat, frame_rect, frame_count});
            }
        }
    }

    std::pair<Drawable *, DrawableAnimation *> create()
    {
        std::pair<Sprite *, SpriteAnimation *> sprite = {nullptr, nullptr};

        sprite.first = new Sprite{texture, texture_rect};

        if (!animations.empty())
        {
            sprite.second = new SpriteAnimation{sprite.first};
            for (const auto &it : animations)
            {
                sprite.second->addState(
                    it.first,
                    sf::milliseconds(it.second.interval),
                    it.second.repeat,
                    it.second.frame_rect,
                    it.second.frame_count);
            }
        }

        return sprite;
    }

    struct Animation
    {
        int32_t interval = 0;
        bool repeat = false;
        sf::IntRect frame_rect;
        sf::Vector2i frame_count;
    };

    std::string name;
    sf::Texture *texture = nullptr;
    sf::IntRect texture_rect;
    std::unordered_map<std::string, Animation> animations;
};

KNOWLEDGE_BASE_REGISTER_DRAWABLE(Sprite, SpriteAnimation);

} // namespace fck

#endif // KNOWLEDGEBASE_H
