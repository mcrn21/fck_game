#ifndef KYOSHISCRIPT_H
#define KYOSHISCRIPT_H

#include "../entity_script_base.h"

namespace fck
{

class KyoshiScript : public EntityScriptBase
{
public:
    KyoshiScript();
    ~KyoshiScript() = default;

    void update(const Entity &entity, double delta_time);
};

} // namespace fck

#endif // KYOSHISCRIPT_H
