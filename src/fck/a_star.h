#ifndef A_STAR_FTXEOUQWCTXW_H
#define A_STAR_FTXEOUQWCTXW_H

#include "grid.h"

#include "SFML/System/Vector2.hpp"

#include <cstdint>
#include <functional>

namespace fck
{

class WorldGrid;

class PathFinder
{
public:
    struct Node
    {
        Node(const sf::Vector2i coordinates, Node *parent = nullptr);

        uint32_t score();

        sf::Vector2i coordinates;
        Node *parent;
        uint32_t g;
        uint32_t h;
    };

    struct Heuristic
    {
        static sf::Vector2i delta(const sf::Vector2i &source, const sf::Vector2i &target);
        static uint32_t manhattan(const sf::Vector2i &source, const sf::Vector2i &target);
        static uint32_t euclidean(const sf::Vector2i &source, const sf::Vector2i &target);
        static uint32_t octagonal(const sf::Vector2i &source, const sf::Vector2i &target);
    };

    PathFinder(const Grid<int32_t> &walls);
    ~PathFinder() = default;

    const Grid<int32_t> *getWalls() const;
    void setWalls(const Grid<int32_t> &grid);

    void setHeuristic(
        const std::function<uint32_t(const sf::Vector2i &, const sf::Vector2i &)> &heuristic);
    void setDiagonalMovement(bool enable);

    std::vector<sf::Vector2i> findPath(const sf::Vector2i &source, const sf::Vector2i &target);

private:
    bool detectCollision(const sf::Vector2i coordinates);
    Node *findNodeOnList(const std::vector<Node *> &nodes, const sf::Vector2i coordinates);
    void releaseNodes(std::vector<Node *> &nodes);

private:
    static std::vector<sf::Vector2i> m_directions;

    const Grid<int32_t> *m_walls;
    std::function<uint32_t(const sf::Vector2i &, const sf::Vector2i &)> m_heuristic;
    int32_t m_directions_count;
};

} // namespace fck

#endif // A_STAR_FTXEOUQWCTXW_H
