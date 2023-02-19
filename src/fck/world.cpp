#include "world.h"
#include "common.h"
#include "utilities.h"

namespace fck
{

template<class T>
void ensureCapacity(T &container, typename T::size_type index)
{
    if (container.size() <= index)
        container.resize(index + 1);
}

World::World(int32_t size) : m_entity_id_storage{size}, m_entity_attributes{size}
{
}

World::~World()
{
}

void World::removeAllSystems()
{
    for (auto &it : m_systems)
        it.second->m_entities.clear();

    m_systems.clear();
}

Entity World::createEntity()
{
    checkForResize(1);

    m_entity_cache.alive.emplace_back(m_entity_id_storage.create(), this);
    return m_entity_cache.alive.back();
}

std::vector<Entity> World::createEntities(int32_t size)
{
    std::vector<Entity> entities;
    entities.reserve(size);

    checkForResize(size);

    for (int32_t i = 0; i < size; ++i)
    {
        Entity e{m_entity_id_storage.create(), this};
        m_entity_cache.alive.push_back(e);
        entities.push_back(e);
    }

    return entities;
}

void World::destroyEntity(const Entity &entity)
{
    if (!entity.isValid())
        return;

    disableEntity(entity);
    m_entity_cache.destroyed.push_back(entity);
}

void World::destroyEntities(const std::vector<Entity> &entities)
{
    for (auto &entity : entities)
        destroyEntity(entity);
}

void World::destroyAllEntities()
{
    destroyEntities(m_entity_cache.alive);
}

int32_t World::getEntityCount() const
{
    return m_entity_cache.alive.size();
}

const std::vector<Entity> &World::getEntities() const
{
    return m_entity_cache.alive;
}

Entity World::getEntity(std::size_t index)
{
    return Entity{m_entity_id_storage.get(index), this};
}

void World::enableEntity(const Entity &entity)
{
    if (!isValid(entity))
    {
        spdlog::warn("Enable invalid entity: id: {}", entity.getId().getIndex());
        return;
    }

    m_entity_cache.enabled.push_back(entity);
}

void World::disableEntity(const Entity &entity)
{
    if (!isValid(entity))
    {
        spdlog::warn("Disable invalid entity: id: {}", entity.getId().getIndex());
        return;
    }

    m_entity_cache.disabled.push_back(entity);
}

bool World::isEnabled(const Entity &entity) const
{
    return isValid(entity) ? m_entity_attributes.attributes[entity.getId().getIndex()].enabled
                           : false;
}

bool World::isValid(const Entity &entity) const
{
    return m_entity_id_storage.isValid(entity.getId());
}

void World::refresh()
{
    // go through all the activated entities from last call to refresh
    for (auto &entity : m_entity_cache.enabled)
    {
        if (!entity.isValid())
            continue;

        auto &attribute = m_entity_attributes.attributes[entity.getId().getIndex()];
        attribute.enabled = true;

        // loop through all the systems within the scene
        for (auto &it : m_systems)
        {
            uint64_t system_index = it.first;

            // if the entity passes the filter the system has and is not already part of the system
            if (m_entity_attributes.component_storage.getComponentsFilter(entity).test(
                    it.second->getComponentsFilter()))
            {
                if (attribute.systems.size() <= system_index || !attribute.systems[system_index])
                {
                    it.second->addEntity(entity);
                    ensureCapacity(attribute.systems, system_index);
                    attribute.systems[system_index] = true;
                }
            }
            else if (attribute.systems.size() > system_index && attribute.systems[system_index])
            {
                it.second->removeEntity(entity);
                attribute.systems[system_index] = false;
            }
        }

        entity_enabled(entity);
    }

    // go through all the deactivated entities from last call to refresh
    for (auto &entity : m_entity_cache.disabled)
    {
        if (!entity.isValid())
            continue;

        auto &attribute = m_entity_attributes.attributes[entity.getId().getIndex()];
        attribute.enabled = false;

        // loop through all the systems within the world
        for (auto &it : m_systems)
        {
            uint64_t system_index = it.first;

            if (attribute.systems.size() <= system_index)
                continue;

            if (attribute.systems[system_index])
            {
                it.second->removeEntity(entity);
                attribute.systems[system_index] = false;
            }
        }

        entity_disabled(entity);
    }

    // go through all the killed entities from last call to refresh
    for (auto &entity : m_entity_cache.destroyed)
    {
        if (!entity.isValid())
            continue;

        entity_destroyed(entity);

        m_entity_cache.alive.erase(
            std::remove(m_entity_cache.alive.begin(), m_entity_cache.alive.end(), entity),
            m_entity_cache.alive.end());

        m_entity_attributes.component_storage.removeAll(entity);
        m_entity_id_storage.destroy(entity.getId());
    }

    m_entity_cache.clearTemp();
}

void World::clear()
{
    removeAllSystems();

    m_entity_attributes.clear();
    m_entity_cache.clear();
    m_entity_id_storage.clear();
}

void World::addSystem(SystemBase &system, TypeId system_type_id)
{
    fck_assert(!system.m_world, "System is already contained within a Scene");
    fck_assert(
        m_systems.count(system_type_id) == 0,
        "System of this type is already contained within the world");

    m_systems[system_type_id].reset(&system);

    system.m_world = this;
    system.initialize();
}

void World::removeSystem(TypeId system_type_id)
{
    fck_assert(systemExist(system_type_id), "System does not exist in world");
    m_systems[system_type_id]->m_entities.clear();
    m_systems.erase(system_type_id);
}

bool World::systemExist(TypeId system_type_id) const
{
    return m_systems.find(system_type_id) != m_systems.end();
}

void World::checkForResize(int32_t size)
{
    auto new_size = getEntityCount() + size;
    if (new_size > m_entity_id_storage.size())
        resize(new_size);
}

void World::resize(int32_t size)
{
    m_entity_id_storage.resize(size);
    m_entity_attributes.resize(size);
}

} // namespace fck
