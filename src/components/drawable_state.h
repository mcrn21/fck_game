#ifndef DRAWABLE_STATE_H
#define DRAWABLE_STATE_H

#include "../fck/drawable_state.h"

#include <memory>

namespace fck::component
{

struct DrawableState
{
    std::unique_ptr<fck::DrawableState> state;
};

} // namespace fck::component

#endif // DRAWABLE_STATE_H
