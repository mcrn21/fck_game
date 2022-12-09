#ifndef SCRIPT_DAWQCBFULHWB_H
#define SCRIPT_DAWQCBFULHWB_H

#include "../components/components.h"

#include "../fck/a_star.h"
#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck::system
{

class Script : public System<component::Script>
{
public:
    Script();
    ~Script() = default;

    void update(double delta_time);
};

} // namespace fck::system

#endif // SCRIPT_DAWQCBFULHWB_H
