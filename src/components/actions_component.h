#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "../action_base.h"

#include <memory>
#include <vector>

namespace fck
{

struct ActionsComponent
{
    int32_t next_action = -1;
    std::vector<std::unique_ptr<ActionBase>> actions;
};

} // namespace fck

#endif // ACTION_COMPONENT_H
