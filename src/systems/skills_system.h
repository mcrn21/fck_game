#ifndef SKILLSSYSTEM_H
#define SKILLSSYSTEM_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck
{

class SkillsSystem : public System<SkillsComponent>
{
public:
    SkillsSystem();
    ~SkillsSystem() = default;

    void update(double delta_time);
};

} // namespace fck

#endif // SKILLSSYSTEM_H
