#ifndef RENDER_FIOOCKRNAESM_H
#define RENDER_FIOOCKRNAESM_H

#include "../components/components.h"

#include "../fck/b2_dynamic_tree.h"
#include "../fck/system.h"

namespace fck::system
{

class Render : public System<component::Drawable, component::Transform>
{
public:
    Render(b2::DynamicTree<Entity> *tree);
    ~Render() = default;

    void moveEntity(const Entity &entity, const sf::Vector2f &offset);

protected:
    void onEntityAdded(Entity &entity);
    void onEntityRemoved(Entity &entity);

private:
    b2::DynamicTree<Entity> *m_tree;
};

} // namespace fck::system

#endif // RENDER_FIOOCKRNAESM_H
