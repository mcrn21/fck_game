#ifndef TARGETSYSTEM_H
#define TARGETSYSTEM_H

#include "../fck_common.h"

#include "../components/components.h"

#include "../fck/a_star.h"
#include "../fck/system.h"

namespace fck
{

class TargetFollowSystem : public System<
                               TargetComponent,
                               VelocityComponent,
                               TransformComponent,
                               StateComponent,
                               TargetFollowComponent,
                               SceneComponent>
{
public:
    TargetFollowSystem(PathFinder *path_finder);
    ~TargetFollowSystem() = default;

    void update(double delta_time);

private:
    PathFinder *m_path_finder;
};

} // namespace fck

#endif // TARGETSYSTEM_H
