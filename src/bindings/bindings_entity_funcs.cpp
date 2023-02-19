#include "../entity_funcs.h"
#include "bindings.h"

namespace fck
{

void bindEntityFuncsToLua(sol::state &sol_state, sol::usertype<Entity> &entity_bind)
{
    entity_bind["move"] = &entity_funcs::move;
    entity_bind["setPosition"] = &entity_funcs::setPosition;
    entity_bind["setParent"] = &entity_funcs::setParent;
    entity_bind["setState"] = &entity_funcs::setState;
    entity_bind["setDirection"] = &entity_funcs::setDirection;
    entity_bind["setTarget"] = &entity_funcs::setTarget;
    entity_bind["setMarker"] = &entity_funcs::setMarker;
    entity_bind["setDrawableState"] = &entity_funcs::setDrawableState;
    entity_bind["playSound"] = &entity_funcs::playSound;
    entity_bind["stopSound"] = &entity_funcs::stopSound;
    entity_bind["stopAllSound"] = &entity_funcs::stopAllSound;
    entity_bind["setScript"] = &entity_funcs::setScript;
}

} // namespace fck
