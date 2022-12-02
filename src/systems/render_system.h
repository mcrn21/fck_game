#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../components/components.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck
{

class RenderSystem : public System<DrawableComponent>
{
public:
    RenderSystem(b2::DynamicTree<Entity> *tree);
    ~RenderSystem() = default;

    void moveEntity(const Entity &entity, const sf::Vector2f &offset);

protected:
    void onEntityAdded(Entity &entity);
    void onEntityRemoved(Entity &entity);

private:
    b2::DynamicTree<Entity> *m_tree;
};

} // namespace fck

#endif // RENDERSYSTEM_H
