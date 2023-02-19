#ifndef LOOKAROUND_ZJDSJQCZZTNO_H
#define LOOKAROUND_ZJDSJQCZZTNO_H

#include "../components/components.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck::system
{

class LookAround : public System<component::LookAround, component::Transform, component::State>
{
public:
    LookAround(b2::DynamicTree<Entity> *tree);
    ~LookAround() = default;

    void update(double delta_time);

    void updateBounds(const Entity &entity);

private:
    b2::DynamicTree<Entity> *m_tree;
};

} // namespace fck::system

#endif // LOOKAROUND_ZJDSJQCZZTNO_H
