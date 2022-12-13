#include "a_star.h"

#include <cmath>

namespace fck
{

PathFinder::Node::Node(const sf::Vector2i coordinates, Node *parent)
    : coordinates{coordinates}, parent{parent}, g{0}, h{0}
{
}

uint32_t PathFinder::Node::score()
{
    return g + h;
}

sf::Vector2i PathFinder::Heuristic::delta(const sf::Vector2i &source, const sf::Vector2i &target)
{
    return {std::abs(source.x - target.x), std::abs(source.y - target.y)};
}

uint32_t PathFinder::Heuristic::manhattan(const sf::Vector2i &source, const sf::Vector2i &target)
{
    sf::Vector2i d = std::move(delta(source, target));
    return uint32_t(10 * (d.x + d.y));
}

uint32_t PathFinder::Heuristic::euclidean(const sf::Vector2i &source, const sf::Vector2i &target)
{
    sf::Vector2i d = std::move(delta(source, target));
    return uint32_t(10 * std::sqrt(std::pow(d.x, 2) + std::pow(d.y, 2)));
}

uint32_t PathFinder::Heuristic::octagonal(const sf::Vector2i &source, const sf::Vector2i &target)
{
    sf::Vector2i d = std::move(delta(source, target));
    return 10 * (d.x + d.y) + (-6) * std::min(d.x, d.y);
}

PathFinder::PathFinder()
    : m_heuristic{&Heuristic::manhattan},
      m_directions_count{8},
      m_directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}}
{
}

Grid<PathFinder::Cell> &PathFinder::getGrid()
{
    return m_grid;
}

void PathFinder::setHeuristic(
    const std::function<uint32_t(const sf::Vector2i &, const sf::Vector2i &)> &heuristic)
{
    m_heuristic = heuristic;
}

void PathFinder::setDiagonalMovement(bool enable)
{
    m_directions_count = enable ? 8 : 4;
}

std::vector<sf::Vector2i> PathFinder::findPath(
    const sf::Vector2i &source, const sf::Vector2i &target)
{
    if (detectCollision(target))
        return {};

    Node *current = nullptr;
    std::vector<Node *> opened;
    std::vector<Node *> closed;
    opened.reserve(100);
    closed.reserve(100);
    opened.push_back(new Node(source));

    while (!opened.empty())
    {
        auto current_it = opened.begin();
        current = *current_it;

        for (auto it = opened.begin(); it != opened.end(); it++)
        {
            auto node = *it;
            if (node->score() <= current->score())
            {
                current = node;
                current_it = it;
            }
        }

        if (current->coordinates == target)
            break;

        closed.push_back(current);
        opened.erase(current_it);

        for (uint i = 0; i < m_directions_count; ++i)
        {
            sf::Vector2i new_coordinates(current->coordinates + m_directions[i]);
            if (detectCollision(new_coordinates) || findNodeOnList(closed, new_coordinates))
            {
                continue;
            }

            uint totalCost = current->g + ((i < 4) ? 10 : 14);

            Node *successor = findNodeOnList(opened, new_coordinates);
            if (successor == nullptr)
            {
                successor = new Node(new_coordinates, current);
                successor->g = totalCost;
                successor->h = m_heuristic(successor->coordinates, target);
                opened.push_back(successor);
            }
            else if (totalCost < successor->g)
            {
                successor->parent = current;
                successor->g = totalCost;
            }
        }
    }

    std::vector<sf::Vector2i> path;
    while (current != nullptr)
    {
        path.push_back(current->coordinates);
        current = current->parent;
    }

    releaseNodes(opened);
    releaseNodes(closed);

    return path;
}

bool PathFinder::detectCollision(const sf::Vector2i coordinates)
{
    Cell *cell = m_grid.getCell(coordinates);
    if (cell)
        return cell->weight > 0;

    return false;
}

PathFinder::Node *PathFinder::findNodeOnList(
    const std::vector<Node *> &nodes, const sf::Vector2i coordinates)
{
    for (auto node : nodes)
    {
        if (node->coordinates == coordinates)
            return node;
    }
    return nullptr;
}

void PathFinder::releaseNodes(std::vector<Node *> &nodes)
{
    for (auto it = nodes.begin(); it != nodes.end();)
    {
        delete *it;
        it = nodes.erase(it);
    }
}

} // namespace fck
