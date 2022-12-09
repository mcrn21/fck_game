#ifndef ENTITY_IVVPWAPMUTXK_H
#define ENTITY_IVVPWAPMUTXK_H

#include "component.h"
#include "id_storage.h"

namespace fck
{
class World;

class Entity
{
public:
    Entity();
    Entity(Id id, World *scene);
    Entity(const Entity &other);

    Entity &operator=(const Entity &other);

    const Id &id() const;
    World *world() const;

    bool isValid() const;
    bool isEnabled() const;

    void enable();
    void disable();
    void destroy();

    template<typename T, typename... Args>
    T &addComponent(Args &&...args);

    template<typename T>
    void removeComponent();

    void removeAllComponents();

    template<typename T>
    T &component() const;

    template<typename T>
    bool hasComponent() const;

    ComponentsFilter componentFilter() const;

    bool operator==(const Entity &entity) const;
    bool operator!=(const Entity &entity) const;

private:
    void addComponent(ComponentBase *component, TypeId component_type_id);
    void removeComponent(TypeId component_type_id);
    ComponentBase *component(TypeId component_type_id) const;
    bool hasComponent(TypeId component_type_id) const;

private:
    Id m_id;
    World *m_world;
};

template<typename T, typename... Args>
T &Entity::addComponent(Args &&...args)
{
    Component<T> *component = new Component<T>{new T{std::forward<Args>(args)...}};
    addComponent(component, componentTypeId<T>());
    return *(component->data.get());
}

template<typename T>
void Entity::removeComponent()
{
    removeComponent(componentTypeId<T>());
}

template<typename T>
T &Entity::component() const
{
    Component<T> *c = static_cast<Component<T> *>(component(componentTypeId<T>()));
    return static_cast<T &>(*(c->data.get()));
}

template<typename T>
bool Entity::hasComponent() const
{
    return hasComponent(componentTypeId<T>());
}

} // namespace fck

#endif // ENTITY_IVVPWAPMUTXK_H
