#ifndef SOUND_LKLKOEOAWRGU_H
#define SOUND_LKLKOEOAWRGU_H

#include "../fck_common.h"
#include "../knowledge_base.h"

#include "../fck/resource_cache.h"

#include <SFML/Audio.hpp>

#include "unordered_map"

namespace fck
{

namespace component
{

struct Sound
{
    struct SoundBuffer
    {
        sf::SoundBuffer *sound_buffer = nullptr;
        bool loop = false;
        float pitch = 1.0f;
    };

    tile_material_type::Type tile_material;
    std::unordered_map<std::string, SoundBuffer> sound_buffers;
    std::string current_sound;
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
        for (const auto &it : *table)
        {
            spdlog::debug("sound {}", it.first.data());
            if (it.second.is_table())
            {
                toml::table *sound_table = it.second.as_table();

                component::Sound::SoundBuffer sound_buffer;

                sound_buffer.sound_buffer = ResourceCache::getResource<sf::SoundBuffer>(
                    sound_table->get("sound_buffer")->as_string()->get());

                if (sound_table->contains("loop"))
                    sound_buffer.loop = sound_table->at("loop").as_boolean()->get();

                if (sound_table->contains("pitch"))
                    sound_buffer.pitch = sound_table->at("pitch").as_floating_point()->get();

                sound_buffers.emplace(it.first.data(), sound_buffer);
            }
        }
    }

    void create(Entity &entity)
    {
        component::Sound &component = entity.addComponent<component::Sound>();
        component.sound_buffers = sound_buffers;
    }

    std::unordered_map<std::string, component::Sound::SoundBuffer> sound_buffers;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Sound);

}

} // namespace fck

#endif // SOUNDCOMPONENT_LKLKOEOAWRGU_H
