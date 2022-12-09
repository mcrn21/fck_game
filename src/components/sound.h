#ifndef SOUND_LKLKOEOAWRGU_H
#define SOUND_LKLKOEOAWRGU_H

#include "../knowledge_base.h"

#include <SFML/Audio.hpp>

namespace fck
{

namespace component
{

struct Sound
{
    sf::Sound sound;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Sound> : ComponentItemBase
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
        component::Sound &component = entity.addComponent<component::Sound>();
    }
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Sound);

}

} // namespace fck

#endif // SOUNDCOMPONENT_LKLKOEOAWRGU_H
