#ifndef SCENE_TJFEPRLODBPB_H
#define SCENE_TJFEPRLODBPB_H

#include "../components/components.h"

#include "../fck/a_star.h"
#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck::system
{

class Scene : public System<component::Scene, component::Transform>
{
public:
    Scene(b2::DynamicTree<Entity> *tree);
    ~Scene() = default;

public: // slots
    void onEntityMoved(const Entity &entity, const sf::Vector2f &offset);

protected:
    void onEntityAdded(Entity &entity);
    void onEntityRemoved(Entity &entity);

private:
    b2::DynamicTree<Entity> *m_tree;
};

} // namespace fck::system

#endif // SCENE_TJFEPRLODBPB_H
