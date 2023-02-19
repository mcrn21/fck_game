#ifndef DRAWABLEFACTORY_XAXFJNNODRMX_H
#define DRAWABLEFACTORY_XAXFJNNODRMX_H

#include "../fck/drawable_animation.h"
#include "../fck/drawable_proxy.h"
#include "../fck/drawable_state.h"
#include "../fck/resource_cache.h"
#include "../fck/sprite_animation.h"
#include "../fck/sprite_state.h"
#include "../fck/utilities.h"
#include "../fck_common.h"

#include <spdlog/spdlog.h>
#include <toml++/toml.h>

#include <functional>
#include <unordered_map>

namespace fck
{

#define REGISTER_DRAWABLE_FACTORY(_drawable_type_, _class1_) \
    inline const bool drawable_factory_##_class1_ \
        = ::fck::DrawableFactory::registerDrawableFactory<_class1_>(_drawable_type_)

class DrawableFactory
{
public:
    struct Factory
    {
        virtual std::tuple<DrawableProxyBase *, DrawableState *, DrawableAnimation *> fromToml(
            toml::table *table)
            = 0;
    };

    template<typename T>
    static bool registerDrawableFactory(drawable_type::Type drawable_type);

    static std::tuple<DrawableProxyBase *, DrawableState *, DrawableAnimation *> createDrawable(
        drawable_type::Type drawable_type, toml::table *table);

private:
    static DrawableFactory &instance();

    DrawableFactory() = default;
    ~DrawableFactory() = default;

private:
    std::unordered_map<drawable_type::Type, std::unique_ptr<Factory>> m_loaders;
};

template<typename T>
bool DrawableFactory::registerDrawableFactory(drawable_type::Type drawable_type)
{
    spdlog::info("Register drawable: {}", drawable_type::toString(drawable_type));
    instance().m_loaders.emplace(drawable_type, std::make_unique<T>());
    return true;
}

struct SpriteFactory : public DrawableFactory::Factory
{
    std::tuple<DrawableProxyBase *, DrawableState *, DrawableAnimation *> fromToml(
        toml::table *table)
    {
        std::unique_ptr<sf::Sprite> sprite = std::make_unique<sf::Sprite>();

        std::string texture_name = table->at("texture").as_string()->get();
        sf::Texture *texture = ResourceCache::get<sf::Texture>(texture_name);
        if (!texture)
        {
            spdlog::warn("Can't load sprite: texture not found: {}", texture_name);
            return {nullptr, nullptr, nullptr};
        }

        sprite->setTexture(*texture);

        sf::IntRect texture_rect;
        if (table->contains("texture_rect"))
            texture_rect = rect::tomlArrayToIntRect(table->at("texture_rect").as_array());

        sprite->setTextureRect(texture_rect);

        for (auto &it : *table)
        {
            if (it.second.is_table())
                continue;

            std::string field_name = it.first.data();

            if (field_name == "position")
                sprite->setPosition(vector2::tomlArrayToVector2f(it.second.as_array()));

            if (field_name == "rotation")
                sprite->setRotation(sf::degrees(it.second.as_floating_point()->get()));

            if (field_name == "scale")
                sprite->setScale(vector2::tomlArrayToVector2f(it.second.as_array()));

            if (field_name == "origin")
                sprite->setOrigin(vector2::tomlArrayToVector2f(it.second.as_array()));
        }

        std::unique_ptr<SpriteState> sprite_state;
        if (table->contains("states") && table->at("states").is_table())
        {
            sprite_state = std::make_unique<SpriteState>(*sprite);
            toml::table *states_table = table->at("states").as_table();

            for (auto &it : *states_table)
            {
                if (!it.second.is_table())
                    continue;

                toml::table *state_table = it.second.as_table();

                sf::IntRect state_rect
                    = rect::tomlArrayToIntRect(state_table->at("texture_rect").as_array());

                sprite_state->addState(it.first.data(), state_rect);
            }
        }

        std::unique_ptr<SpriteAnimation> sprite_animation;
        if (table->contains("animations") && table->at("animations").is_table())
        {
            sprite_animation = std::make_unique<SpriteAnimation>(*sprite);
            toml::table *abimations_table = table->at("animations").as_table();

            for (auto &it : *abimations_table)
            {
                if (!it.second.is_table())
                    continue;

                toml::table *state_table = it.second.as_table();

                int32_t interval = state_table->at("interval").as_integer()->get();
                bool repeat = state_table->at("repeat").as_boolean()->get();
                sf::IntRect texture_rect
                    = rect::tomlArrayToIntRect(state_table->at("texture_rect").as_array());
                sf::Vector2i frame_count
                    = vector2::tomlArrayToVector2i(state_table->at("frame_count").as_array());

                sprite_animation->addState(
                    it.first.data(), sf::milliseconds(interval), repeat, texture_rect, frame_count);
            }
        }

        return {
            new DrawableProxy<sf::Sprite>{sprite.release()},
            sprite_state.release(),
            sprite_animation.release()};
    }
};

REGISTER_DRAWABLE_FACTORY(drawable_type::SPRITE, SpriteFactory);

} // namespace fck

#endif // DRAWABLEFACTORY_XAXFJNNODRMX_H
