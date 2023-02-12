#ifndef TARGETFOLLOW_UDCGQLCESNUY_H
#define TARGETFOLLOW_UDCGQLCESNUY_H

#include "../fck_common.h"

#include "../components/components.h"

#include "../fck/system.h"
#include "../fck/vector_2d.h"

namespace fck::system
{

class TargetFollow : public System<
                         component::Velocity,
                         component::Transform,
                         component::TargetFollow,
                         component::State>
{
public:
    TargetFollow();
    ~TargetFollow() = default;

    const Vector2D<int32_t> *getWalls() const;
    void setWalls(const Vector2D<int32_t> &walls);

    const sf::Vector2i &getWallSize() const;
    void setWallSize(const sf::Vector2i &wall_size);

    void clearWalls();

    void update(double delta_time);

private:
    sf::Vector2i transformPosition(const sf::Vector2f &position);

private:
    const Vector2D<int32_t> *m_walls;
    sf::Vector2i m_wall_size;
};

} // namespace fck::system

#endif // TARGETFOLLOW_UDCGQLCESNUY_H
