#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "fck/entity.h"

#include <SFML/Graphics.hpp>

namespace fck::debug_draw
{

void drawEntityId(
    const Entity &entity,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

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

void drawPathFinderCellsBounds(
    const Entity &entity,
    const sf::Vector2i &cell_size,
    sf::RenderTarget &target,
    const sf::RenderStates &states = sf::RenderStates());

void drawTargetFollowPath(
    const Entity &entity,
    const sf::Vector2i &cell_size,
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

#endif // DEBUGDRAW_H
