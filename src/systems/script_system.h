#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../components/components.h"

#include "../fck/a_star.h"
#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck
{

class ScriptSystem : public System<ScriptComponent>
{
public:
    ScriptSystem();
    ~ScriptSystem() = default;

    void update(double delta_time);
};

} // namespace fck

#endif // SCRIPTSYSTEM_H
