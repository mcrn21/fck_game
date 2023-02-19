#ifndef BINDINGS_CRPRHCZFQNZB_H
#define BINDINGS_CRPRHCZFQNZB_H

#include "../fck/entity.h"

#include <sol/sol.hpp>

namespace fck
{

void bindToLua(sol::state &sol_state);
void bindSfmlToLua(sol::state &sol_state);
void bindFckCommonToLua(sol::state &sol_state);
void bindFckToLua(sol::state &sol_state);
sol::usertype<Entity> bindEntityToLua(sol::state &sol_state);
void bindComponentsToLua(sol::state &sol_state, sol::usertype<Entity> &entity_bind);
void bindEntityFuncsToLua(sol::state &sol_state, sol::usertype<Entity> &entity_bind);
void bindMapToLua(sol::state &sol_state);

} // namespace fck

#endif // BINDINGS_CRPRHCZFQNZB_H
