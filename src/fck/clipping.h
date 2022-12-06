#ifndef CLIPPING_H
#define CLIPPING_H

#include <SFML/Graphics.hpp>

namespace fck
{

class Clipping
{

public:
    Clipping(
        sf::RenderTarget &target,
        const sf::RenderStates &states,
        sf::Vector2f top_left,
        sf::Vector2f size);
    Clipping(const Clipping &copy) = delete;
    Clipping &operator=(const Clipping &right) = delete;
    ~Clipping();

    static void setOriginalView(const sf::View &view);

private:
    sf::RenderTarget &m_target;
    sf::View m_old_view;

    static sf::View m_original_view;
};

} // namespace fck

#endif // CLIPPING_H
