#ifndef TARGETFOLLOW_UDCGQLCESNUY_H
#define TARGETFOLLOW_UDCGQLCESNUY_H

#include "../fck_common.h"

#include "../components/components.h"

#include "../fck/a_star.h"
#include "../fck/system.h"

namespace fck::system
{

class TargetFollow : public System<
                         component::Target,
                         component::Velocity,
                         component::Transform,
                         component::State,
                         component::TargetFollow,
                         component::Scene>
{
public:
    TargetFollow(PathFinder *path_finder);
    ~TargetFollow() = default;

    void update(double delta_time);

private:
    PathFinder *m_path_finder;
};

} // namespace fck::system

#endif // TARGETFOLLOW_UDCGQLCESNUY_H
