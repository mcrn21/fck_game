#ifndef SKILLS_INIXVFWWRLWI_H
#define SKILLS_INIXVFWWRLWI_H

#include "../components/components.h"

#include "../fck/system.h"

namespace fck::system
{

class Skills : public System<component::Skills>
{
public:
    Skills();
    ~Skills() = default;

    void update(double delta_time);
};

} // namespace fck::system

#endif // SKILLS_INIXVFWWRLWI_H
