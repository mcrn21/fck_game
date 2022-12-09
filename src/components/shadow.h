#ifndef SHADOW_MNLVCFKJEGDV_H
#define SHADOW_MNLVCFKJEGDV_H

#include "../knowledge_base.h"

#include "../fck/resource_cache.h"

#include <SFML/Graphics.hpp>

namespace fck
{

namespace component
{

struct Shadow
{
    sf::Sprite shadow;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Shadow> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SHADOW;
    }

    void init(toml::table *table)
    {
        if (table->contains("texture"))
            texture = table->at("texture").as_string()->get();

        if (table->contains("scale"))
            scale = vector2::tomlArrayToVector2f(table->at("scale").as_array());
    }

    void create(Entity &entity)
    {
        component::Shadow &component = entity.addComponent<component::Shadow>();

        sf::Texture *tex = ResourceCache::resource<sf::Texture>(texture);
        if (tex)
        {
            component.shadow.setTexture(*tex, true);
            component.shadow.setOrigin(component.shadow.getLocalBounds().getSize() / 2.0f);
            component.shadow.setScale(scale);
        }
    }

    std::string texture;

    sf::Vector2f scale = {1.0f, 1.0f};
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Shadow);

}

} // namespace fck

#endif // SHADOW_MNLVCFKJEGDV_H
