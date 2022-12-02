#include "kyoshi_script.h"

#include "../components/components.h"

namespace fck
{

KyoshiScript::KyoshiScript()
{
}

void KyoshiScript::update(const Entity &entity, double delta_time)
{
    TargetFollowComponent &target_follow_component = entity.component<TargetFollowComponent>();

    //    if (target_follow_component.state == TargetFollowComponent::RICHED)
    //    {
    //        actions_component.next_action = 0;
    //    }
}

} // namespace fck
