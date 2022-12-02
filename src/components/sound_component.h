#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include "../knowledge_base.h"

#include <SFML/Audio.hpp>

namespace fck
{

struct SoundComponent
{
    sf::Sound sound;
};

template<>
struct KnowledgeBase::ComponentItem<SoundComponent> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SOUND;
    }

    void init(toml::table *table)
    {
    }

    void create(Entity &entity)
    {
        SoundComponent &component = entity.addComponent<SoundComponent>();
    }
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(SoundComponent);

} // namespace fck

#endif // SOUNDCOMPONENT_H
