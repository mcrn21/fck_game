#ifndef TARGETFOLLOW_UDCGQLCESNUY_H
#define TARGETFOLLOW_UDCGQLCESNUY_H

#include "../components/components.h"
#include "../fck/system.h"
#include "../fck/vector_2d.h"
#include "../fck_common.h"
#include "../map/map.h"

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

    void update(double delta_time);

public: //slots
    void onMapChanged(map::Map *map);
    void onChunkChanged(const sf::Vector2i &chunk_coords);

private:
    sf::Vector2i transformPosition(const sf::Vector2f &position);

private:
    map::Map *m_map;
    const Vector2D<int32_t> *m_walls;
    sf::Vector2i m_wall_size;
};

} // namespace fck::system

#endif // TARGETFOLLOW_UDCGQLCESNUY_H
