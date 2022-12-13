#include "component_storage.h"
#include "common.h"
#include "utilities.h"

namespace fck
{

ComponentStorage::ComponentStorage(int32_t size) : m_component_entries(size)
{
}

void ComponentStorage::addComponent(
    Entity &entity, ComponentBase *component, TypeId component_type_id)
{
    fck_assert(entity.isValid(), "invalid entity cannot have components added to it");

    auto index = entity.getId().getIndex();
    auto &component_data_for_entity = m_component_entries[index];

    component_data_for_entity.components[component_type_id].reset(component);
    component_data_for_entity.components_filter.filter |= (1 << component_type_id);
}

void ComponentStorage::removeComponent(Entity &entity, TypeId component_type_id)
{
    if (!entity.isValid())
        return;

    auto index = entity.getId().getIndex();
    auto &component_data_for_entity = m_component_entries[index];

    component_data_for_entity.components[component_type_id].reset();
    component_data_for_entity.components_filter.filter &= ~(1 << component_type_id);
}

void ComponentStorage::removeAllComponents(Entity &entity)
{
    if (!entity.isValid())
        return;

    auto index = entity.getId().getIndex();
    auto &component_data_for_entity = m_component_entries[index];

    for (int32_t i = 0; i < MAX_AMOUNT_OF_COMPONENTS; ++i)
    {
        if (component_data_for_entity.components_filter.filter & (1 << i))
        {
            component_data_for_entity.components[i].reset();
            component_data_for_entity.components_filter.filter &= ~(1 << i);
        }
    }
}

ComponentBase *ComponentStorage::getComponent(const Entity &entity, TypeId component_type_id) const
{
    fck_assert(
        entity.isValid() && hasComponent(entity, component_type_id),
        "Entity is not valid or does not contain component");

    return getComponentsArray(entity)[component_type_id].get();
}

ComponentsFilter ComponentStorage::getComponentsFilter(const Entity &entity) const
{
    fck_assert(entity.isValid(), "invalid entity cannot retrieve the component list");

    return m_component_entries[entity.getId().getIndex()].components_filter;
}

std::vector<ComponentBase *> ComponentStorage::getComponents(const Entity &entity) const
{
    fck_assert(entity.isValid(), "invalid entity cannot retrieve components, as it has none");

    auto &components_array = getComponentsArray(entity);

    std::vector<ComponentBase *> components_list;
    components_list.reserve(components_array.size());

    for (auto &component : components_array)
        components_list.emplace_back(component.get());

    return components_list;
}

bool ComponentStorage::hasComponent(const Entity &entity, TypeId component_type_id) const
{
    fck_assert(entity.isValid(), "invalid entity cannot check if it has components");

    auto &components = getComponentsArray(entity);
    return components.size() > component_type_id && components[component_type_id] != nullptr;
}

void ComponentStorage::resize(int32_t size)
{
    m_component_entries.resize(size);
}

void ComponentStorage::clear()
{
    m_component_entries.clear();
}

ComponentStorage::ComponentsArray &ComponentStorage::getComponentsArray(const Entity &e)
{
    return m_component_entries[e.getId().getIndex()].components;
}

const ComponentStorage::ComponentsArray &ComponentStorage::getComponentsArray(const Entity &e) const
{
    return m_component_entries[e.getId().getIndex()].components;
}

} // namespace fck
