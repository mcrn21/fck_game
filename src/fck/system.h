#ifndef SYSTEM_H
#define SYSTEM_H

#include "component.h"
#include "entity.h"

#include <functional>
#include <vector>

namespace fck
{

class World;

class SystemBase
{
    friend class World;

public:
    SystemBase(const ComponentsFilter &components_filter);
    virtual ~SystemBase() = default;

    World *world() const;
    const ComponentsFilter &componentsFilter() const;
    std::vector<Entity> &entities();

    void each(const std::function<void(Entity &)> &callback)
    {
        if (callback)
        {
            auto it = m_entities.begin();
            while (it != m_entities.end())
            {
                callback(*it);
                ++it;
            }
        }
    }

    template<typename T>
    struct EachCallbackType
    {
        using type = T;
    };
    template<typename... Args>
    void each(
        const std::function<void(Entity &, typename EachCallbackType<Args &>::type...)> &callback)
    {
        if (callback)
        {
            auto it = m_entities.begin();
            while (it != m_entities.end())
            {
                callback(*it, (*it).component<Args>()...);
                ++it;
            }
        }
    }

protected:
    virtual void initialize();
    virtual void onEntityAdded(Entity &entity);
    virtual void onEntityRemoved(Entity &entity);

private:
    void setScene(World *scene);
    void addEntity(Entity &entity);
    void removeEntity(Entity &entity);

private:
    World *m_world;
    ComponentsFilter m_components_filter;
    std::vector<Entity> m_entities;
};

template<typename... Args>
class System : public SystemBase
{
public:
    System() : SystemBase{ComponentsFilter::create<Args...>()}
    {
    }
    ~System() = default;
};

template<class T>
TypeId systemTypeId()
{
    return ClassTypeId<SystemBase>::id<T>();
}

} // namespace fck

#endif // SYSTEM_H
