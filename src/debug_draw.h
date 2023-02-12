#ifndef DEBUGDRAW_QZSBRIQHICVO_H
#define DEBUGDRAW_QZSBRIQHICVO_H

#include "fck/entity.h"
#include "map/map.h"

#include <SFML/Graphics.hpp>

namespace fck::debug_draw
{

void drawDrawableBounds(
    const Entity &entity,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

void drawSceneTreeAABB(
    const Entity &entity,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

void drawSceneBounds(
    const Entity &entity,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

void drawVelocity(
    const Entity &entity,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

void drawMapWalls(
    map::Map *map, sf::RenderTarget &target, const sf::RenderStates &states = sf::RenderStates());

void drawTargetFollowPath(
    const Entity &entity,
    map::Map *map,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

void drawLookAroundBound(
    const Entity &entity,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

void drawLookAroundLookBound(
    const Entity &entity,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

} // namespace fck::debug_draw

#endif // DEBUGDRAW_QZSBRIQHICVO_H
