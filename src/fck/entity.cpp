#include "entity.h"
#include "common.h"
#include "utilities.h"
#include "world.h"

namespace fck
{

Entity::Entity() : m_world{nullptr}
{
}

Entity::Entity(Id id, World *scene) : m_id{id}, m_world{scene}
{
}

Entity::Entity(const Entity &other)
{
    m_id = other.m_id;
    m_world = other.m_world;
}

Entity &Entity::operator=(const Entity &other)
{
    m_id = other.m_id;
    m_world = other.m_world;
    return *this;
}

const Id &Entity::getId() const
{
    return m_id;
}

World *Entity::getWorld() const
{
    fck_assert(m_world, "scene reference in entity is null");
    return m_world;
}

bool Entity::isValid() const
{
    return m_world ? m_world->isValid(*this) : false;
}

bool Entity::isEnabled() const
{
    return getWorld()->isEnabled(*this);
}

void Entity::enable()
{
    getWorld()->enableEntity(*this);
}

void Entity::disable()
{
    getWorld()->disableEntity(*this);
}

void Entity::destroy()
{
    getWorld()->destroyEntity(*this);
}

void Entity::removeAllComponents()
{
    getWorld()->m_entity_attributes.component_storage.removeAll(*this);
}

ComponentsFilter Entity::getComponentFilter() const
{
    return getWorld()->m_entity_attributes.component_storage.getComponentsFilter(*this);
}

bool Entity::operator==(const Entity &entity) const
{
    return m_id == entity.m_id && entity.m_world == m_world;
}

bool Entity::operator!=(const Entity &entity) const
{
    return !operator==(entity);
}

void Entity::add(ComponentBase *component, TypeId component_type_id)
{
    getWorld()->m_entity_attributes.component_storage.add(
        *this, component, component_type_id);
}

void Entity::remove(TypeId component_type_id)
{
    getWorld()->m_entity_attributes.component_storage.remove(*this, component_type_id);
}

ComponentBase *Entity::get(TypeId component_type_id) const
{
    return getWorld()->m_entity_attributes.component_storage.get(*this, component_type_id);
}

bool Entity::has(TypeId component_type_id) const
{
    return getWorld()->m_entity_attributes.component_storage.has(*this, component_type_id);
}

} // namespace fck
