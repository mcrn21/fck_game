#include "system.h"

namespace fck
{

SystemBase::SystemBase(const ComponentsFilter &components_filter)
    : m_components_filter{components_filter}, m_world{nullptr}
{
}

World *SystemBase::getWorld() const
{
    return m_world;
}

const ComponentsFilter &SystemBase::getComponentsFilter() const
{
    return m_components_filter;
}

std::vector<Entity> &SystemBase::getEntities()
{
    return m_entities;
}

void SystemBase::initialize()
{
}

void SystemBase::onEntityAdded(Entity &entity)
{
    (void)(entity);
}

void SystemBase::onEntityRemoved(Entity &entity)
{
    (void)(entity);
}

void SystemBase::addEntity(Entity &entity)
{
    m_entities.push_back(entity);
    onEntityAdded(entity);
}

void SystemBase::removeEntity(Entity &entity)
{
    auto entity_found = std::find(m_entities.begin(), m_entities.end(), entity);
    if (entity_found != m_entities.end())
    {
        m_entities.erase(entity_found);
        onEntityRemoved(entity);
    }
}

} // namespace fck
