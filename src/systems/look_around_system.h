#ifndef LOOKAROUNDSYSTEM_H
#define LOOKAROUNDSYSTEM_H

#include "../components/components.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck
{

class LookAroundSystem
    : public System<LookAroundComponent, TransformComponent, StateComponent, TypeComponent>
{
public:
    LookAroundSystem(b2::DynamicTree<Entity> *tree);
    ~LookAroundSystem() = default;

    void update(double delta_time);

    void updateBounds(const Entity &entity);

private:
    b2::DynamicTree<Entity> *m_tree;
};

} // namespace fck

#endif // LOOKAROUNDSYSTEM_H
