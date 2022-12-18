#ifndef TARGETFOLLOW_UDCGQLCESNUY_H
#define TARGETFOLLOW_UDCGQLCESNUY_H

#include "../fck_common.h"

#include "../components/components.h"

#include "../fck/grid.h"
#include "../fck/system.h"

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

    const Grid<int32_t> *getWalls() const;
    void setWalls(const Grid<int32_t> *walls);

    void update(double delta_time);

private:
    const Grid<int32_t> *m_walls;
};

} // namespace fck::system

#endif // TARGETFOLLOW_UDCGQLCESNUY_H
