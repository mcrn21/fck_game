#ifndef COMPONENTSTORAGE_PLZCNBTEUGYS_H
#define COMPONENTSTORAGE_PLZCNBTEUGYS_H

#include "component.h"
#include "entity.h"

namespace fck
{

class ComponentStorage
{

public:
    explicit ComponentStorage(int32_t size);
    ~ComponentStorage() = default;

    ComponentStorage(const ComponentStorage &) = delete;
    ComponentStorage(ComponentStorage &&) = delete;
    ComponentStorage &operator=(const ComponentStorage &) = delete;
    ComponentStorage &operator=(ComponentStorage &&) = delete;

    void addComponent(Entity &entity, ComponentBase *component, TypeId component_type_id);
    void removeComponent(Entity &entity, TypeId component_type_id);
    void removeAllComponents(Entity &entity);

    ComponentBase *component(const Entity &entity, TypeId component_type_id) const;
    ComponentsFilter componentsFilter(const Entity &entity) const;
    std::vector<ComponentBase *> components(const Entity &entity) const;

    bool hasComponent(const Entity &entity, TypeId component_type_id) const;

    void resize(int32_t size);
    void clear();

private:
    typedef std::array<std::unique_ptr<ComponentBase>, MAX_AMOUNT_OF_COMPONENTS> ComponentsArray;

    ComponentsArray &componentsArray(const Entity &e);
    const ComponentsArray &componentsArray(const Entity &e) const;

private:
    struct EntityComponents
    {
        EntityComponents() = default;

        EntityComponents(EntityComponents &&other)
            : components(std::move(other.components)),
              components_filter(std::move(other.components_filter))
        {
        }

        ComponentsArray components;
        ComponentsFilter components_filter;
    };

    std::vector<EntityComponents> m_component_entries;
};

} // namespace fck

#endif // COMPONENTSTORAGE_PLZCNBTEUGYS_H
