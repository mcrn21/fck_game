#ifndef WORLD_SJTWXYDCLHBB_H
#define WORLD_SJTWXYDCLHBB_H

#include "../sigslot/signal.hpp"
#include "component_storage.h"
#include "entity.h"
#include "id_storage.h"
#include "system.h"

#include <unordered_map>
#include <vector>

namespace fck
{

class World
{
    friend class Entity;

public:
    World(int32_t size = 0);
    World(const World &world) = delete;
    World(World &&world) = delete;
    World &operator=(const World &) = delete;
    World &operator=(World &&) = delete;

    ~World();

    template<typename T>
    void addSystem(T &system);

    template<typename T>
    void removeSystem();

    template<typename T>
    bool systemExist() const;

    template<typename T>
    bool systemExist(const T &system) const;

    void removeAllSystems();

    Entity createEntity();
    std::vector<Entity> createEntities(int32_t size);

    void destroyEntity(const Entity &entity);
    void destroyEntities(const std::vector<Entity> &entities);
    void destroyAllEntities();

    int32_t getEntityCount() const;
    const std::vector<Entity> &getEntities() const;
    Entity getEntity(std::size_t index);

    void enableEntity(const Entity &entity);
    void disableEntity(const Entity &entity);
    bool isEnabled(const Entity &entity) const;
    bool isValid(const Entity &entity) const;

    void refresh();
    void clear();

private:
    void addSystem(SystemBase &system, TypeId system_type_id);
    void removeSystem(TypeId system_type_id);
    bool systemExist(TypeId system_type_id) const;

    void checkForResize(int32_t size);
    void resize(int32_t size);

public:
    sigslot::signal<const Entity &> entity_enabled;
    sigslot::signal<const Entity &> entity_disabled;
    sigslot::signal<const Entity &> entity_destroyed;

private:
    struct SystemDeleter
    {
        void operator()(SystemBase *system) const
        {
            system->m_world = nullptr;
            system->m_entities.clear();
        }
    };

    std::unordered_map<TypeId, std::unique_ptr<SystemBase, SystemDeleter>> m_systems;
    IdStorage m_entity_id_storage;

    struct EntityAttributes
    {
        struct Attribute
        {
            bool enabled = false;
            std::vector<bool> systems;
        };

        explicit EntityAttributes(int32_t size = 0) : component_storage(size), attributes(size)
        {
        }

        void resize(int32_t size)
        {
            component_storage.resize(size);
            attributes.resize(size);
        }

        void clear()
        {
            component_storage.clear();
            attributes.clear();
        }

        ComponentStorage component_storage;
        std::vector<Attribute> attributes;
    } m_entity_attributes;

    struct EntityCache
    {
        std::vector<Entity> alive;
        std::vector<Entity> destroyed;
        std::vector<Entity> enabled;
        std::vector<Entity> disabled;

        void clearTemp()
        {
            destroyed.clear();
            enabled.clear();
            disabled.clear();
        }

        void clear()
        {
            alive.clear();
            clearTemp();
        }
    } m_entity_cache;
};

template<typename T>
void World::addSystem(T &system)
{
    static_assert(
        std::is_base_of<SystemBase, T>(), "Template argument does not inherit from SystemBase");
    addSystem(system, systemTypeId<T>());
}

template<typename T>
void World::removeSystem()
{
    static_assert(
        std::is_base_of<SystemBase, T>(), "Template argument does not inherit from SystemBase");
    removeSystem(systemTypeId<T>());
}

template<typename T>
bool World::systemExist() const
{
    static_assert(
        std::is_base_of<SystemBase, T>(), "Template argument does not inherit from SystemBase");
    return systemExist(systemTypeId<T>());
}

template<typename T>
bool World::systemExist(const T &system) const
{
    static_assert(
        std::is_base_of<SystemBase, T>(), "Template argument does not inherit from SystemBase");
    return system.m_scene == this && systemExist<T>();
}

} // namespace fck

#endif // WORLD_SJTWXYDCLHBB_H
