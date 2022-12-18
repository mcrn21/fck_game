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
    tile_material_type::Type tile_material;
    std::unordered_map<std::string, std::unique_ptr<sf::Sound>> sounds;
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

                Sound sound_params;

                sound_params.sound_buffer = ResourceCache::getResource<sf::SoundBuffer>(
                    sound_table->get("sound_buffer")->as_string()->get());

                if (!sound_params.sound_buffer)
                    continue;

                if (sound_table->contains("loop"))
                    sound_params.loop = sound_table->at("loop").as_boolean()->get();

                if (sound_table->contains("pitch"))
                    sound_params.pitch = sound_table->at("pitch").as_floating_point()->get();

                if (sound_table->contains("volume"))
                    sound_params.volume = sound_table->at("volume").as_floating_point()->get();

                if (sound_table->contains("min_distance"))
                    sound_params.min_distance
                        = sound_table->at("min_distance").as_floating_point()->get();

                if (sound_table->contains("attenuation"))
                    sound_params.attenuation
                        = sound_table->at("attenuation").as_floating_point()->get();

                sounds.emplace(it.first.data(), sound_params);
            }
        }
    }

    void create(Entity &entity)
    {
        component::Sound &component = entity.addComponent<component::Sound>();

        for (const auto &it : sounds)
        {
            std::unique_ptr<sf::Sound> sound = std::make_unique<sf::Sound>();
            sound->setBuffer(*it.second.sound_buffer);
            sound->setLoop(it.second.loop);
            sound->setPitch(it.second.pitch);
            sound->setVolume(it.second.volume);
            sound->setMinDistance(it.second.min_distance);
            sound->setAttenuation(it.second.attenuation);

            component.sounds.emplace(it.first, std::move(sound));
        }
    }

    struct Sound
    {
        sf::SoundBuffer *sound_buffer = nullptr;
        bool loop = false;
        float pitch = 1.0f;
        float volume = 100.0f;
        float min_distance = 5.0f;
        float attenuation = 0.12f;
    };

    std::unordered_map<std::string, Sound> sounds;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Sound);

}

} // namespace fck

#endif // SOUNDCOMPONENT_LKLKOEOAWRGU_H
