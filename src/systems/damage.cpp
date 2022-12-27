#include "damage.h"

namespace fck::system
{

Damage::Damage()
{
}

void Damage::update(double delta_time)
{
    for (Entity &entity : getEntities())
    {
        component::Damage &damage_cometent = entity.get<component::Damage>();

        if (damage_cometent.damage)
        {
            component::State &state_component
                = entity.get<component::State>();
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

} // namespace fck::system
