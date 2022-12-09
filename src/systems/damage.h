#ifndef DAMAGE_XEIRUAXOVEWX_H
#define DAMAGE_XEIRUAXOVEWX_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck::system
{

class Damage : public System<component::Damage, component::Stats>
{
public:
    Damage();
    ~Damage() = default;

    void update(double delta_time);
};

} // namespace fck::system

#endif // DAMAGE_XEIRUAXOVEWX_H
