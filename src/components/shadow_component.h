#ifndef SHADOW_COMPONENT_H
#define SHADOW_COMPONENT_H

#include "../knowledge_base.h"

#include "../fck/resource_cache.h"

#include <SFML/Graphics.hpp>

namespace fck
{

struct ShadowComponent
{
    sf::Sprite shadow;
};

template<>
struct KnowledgeBase::ComponentItem<ShadowComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SHADOW;
    }

    void init(toml::table *table)
    {
        if (table->contains("texture"))
            texture = table->at("texture").as_string()->get();
    }

    void create(Entity &entity)
    {
        ShadowComponent &component = entity.addComponent<ShadowComponent>();

        sf::Texture *tex = ResourceCache::resource<sf::Texture>(texture);
        if (tex)
        {
            component.shadow.setTexture(*tex, true);
            component.shadow.setOrigin(component.shadow.getLocalBounds().getSize() / 2.0f);
        }
    }

    std::string texture;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(ShadowComponent);

} // namespace fck

#endif // SHADOW_COMPONENT_H
