#ifndef STATS_YRNVTOQJEIEG_H
#define STATS_YRNVTOQJEIEG_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck::system
{

class Stats : public System<component::Stats, component::State>
{
public:
    Stats();
    ~Stats() = default;

    void update(double delta_time);
};

} // namespace fck::system

#endif // STATS_YRNVTOQJEIEG_H
