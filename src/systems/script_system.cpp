#include "script_system.h"

#include "spdlog/spdlog.h"

namespace fck
{

ScriptSystem::ScriptSystem()
{
}

void ScriptSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        ScriptComponent &script_component = entity.component<ScriptComponent>();
        if (script_component.entity_script)
            script_component.entity_script->update(entity, delta_time);
    }
}

} // namespace fck
