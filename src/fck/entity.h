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

    const Id &getId() const;
    World *getWorld() const;

    bool isValid() const;
    bool isEnabled() const;

    void enable();
    void disable();
    void destroy();

    template<typename T, typename... Args>
    T &add(Args &&...args);

    template<typename T>
    void remove();

    void removeAllComponents();

    template<typename T>
    T &get() const;

    template<typename T>
    bool has() const;

    ComponentsFilter getComponentFilter() const;

    bool operator==(const Entity &entity) const;
    bool operator!=(const Entity &entity) const;

private:
    void add(ComponentBase *component, TypeId component_type_id);
    void remove(TypeId component_type_id);
    ComponentBase *get(TypeId component_type_id) const;
    bool has(TypeId component_type_id) const;

private:
    Id m_id;
    World *m_world;
};

template<typename T, typename... Args>
T &Entity::add(Args &&...args)
{
    Component<T> *component = new Component<T>{new T{std::forward<Args>(args)...}};
    add(component, componentTypeId<T>());
    return *(component->data.get());
}

template<typename T>
void Entity::remove()
{
    remove(componentTypeId<T>());
}

template<typename T>
T &Entity::get() const
{
    Component<T> *c = static_cast<Component<T> *>(get(componentTypeId<T>()));
    return static_cast<T &>(*(c->data.get()));
}

template<typename T>
bool Entity::has() const
{
    return has(componentTypeId<T>());
}

} // namespace fck

#endif // ENTITY_IVVPWAPMUTXK_H
