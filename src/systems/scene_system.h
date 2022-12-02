#ifndef SCENESYSTEM_H
#define SCENESYSTEM_H

#include "../components/components.h"

#include "../fck/a_star.h"
#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck
{

class SceneSystem : public System<SceneComponent, TransformComponent>
{
public:
    SceneSystem(b2::DynamicTree<Entity> *tree, PathFinder *path_finder);
    ~SceneSystem() = default;

    void moveEntity(const Entity &entity, const sf::Vector2f &offset);

protected:
    void onEntityAdded(Entity &entity);
    void onEntityRemoved(Entity &entity);

private:
    b2::DynamicTree<Entity> *m_tree;
    PathFinder *m_path_finder;
};

} // namespace fck

#endif // SCENESYSTEM_H
