#include "bindings.h"

namespace fck
{

void bindToLua(sol::state &sol_state)
{
    // sfml
    bindSfmlToLua(sol_state);

    // fck_common
    bindFckCommonToLua(sol_state);

    // fck
    bindFckToLua(sol_state);
    auto entity_bind = bindEntityToLua(sol_state);

    // components
    bindComponentsToLua(sol_state, entity_bind);

    // entity_funcs
    bindEntityFuncsToLua(sol_state, entity_bind);

    // map
    bindMapToLua(sol_state);
}

} // namespace fck
