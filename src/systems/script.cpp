#include "script.h"

#include "spdlog/spdlog.h"

namespace fck::system
{

Script::Script()
{
}

void Script::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::Script &script_component
            = entity.get<component::Script>();
        if (script_component.entity_script)
            script_component.entity_script->update(entity, delta_time);
    }
}

} // namespace fck::system
