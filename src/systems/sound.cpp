#include "sound.h"

#include <SFML/Audio.hpp>

namespace fck::system
{

Sound::Sound()
{
    // Global value
    sf::Listener::setGlobalVolume(100);
}

void Sound::onEntityMoved(const Entity &entity, const sf::Vector2f &offset)
{
    if (entity.has<component::Player>())
    {
        auto &transform_component = entity.get<component::Transform>();

        sf::Listener::setPosition(
            {transform_component.transform.getPosition().x,
             0,
             transform_component.transform.getPosition().y});

        return;
    }

    if (!entity.has<component::Sound>())
        return;

    //    if (entity.has<component::Sound>())
    //    {
    //        auto &sound_component = entity.get<component::Sound>();

    //        // update tile material for move sound
    //        if (m_map && !vector2::isNegotive(m_map->getCurrentChunkCoords()))
    //        {
    //            const map::Chunk *chunk = m_map->getCurrentChunk();
    //            if (chunk)
    //            {
    //                sf::Vector2i tile_coords
    //                    = m_map->tileCoordsByPosition(transform_component.transform.getPosition());
    //                const map::Tile &tile = chunk->getTiles().getData(tile_coords);

    //                if (sound_component.tile_material != tile.getMaterialType())
    //                {
    //                    sound_component.tile_material = tile.getMaterialType();
    //                }
    //            }
    //        }

    //        // update entity sounds positions
    //        //        for (auto &it : sound_component.sounds)
    //        //        {
    //        //            it.second->setPosition(sf::Vector3f{
    //        //                transform_component.transform.getPosition().x,
    //        //                0,
    //        //                transform_component.transform.getPosition().y});
    //        //        }
    //    }
}

} // namespace fck::system
