#include "damage_system.h"

namespace fck
{

DamageSystem::DamageSystem()
{
}

void DamageSystem::update(double delta_time)
{
    for (Entity &entity : entities())
    {
        DamageComponent &damage_cometent = entity.component<DamageComponent>();

        if (damage_cometent.damage)
        {
            StateComponent &state_component = entity.component<StateComponent>();
            if (state_component.state != entity_state::DEATH)
            {
                damage_cometent.damage->_update(delta_time);
                if (damage_cometent.damage->isReady())
                    damage_cometent.damage.reset();
            }
            else
            {
                damage_cometent.damage.reset();
            }
        }
    }
}

} // namespace fck
