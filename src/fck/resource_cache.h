#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H

#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace fck
{

class ResourceCache
{
public:
    template<typename T>
    static T *resource(const std::string &resource_name);
    template<typename T>
    static void addResource(const std::string &resource_name, T *resource);
    template<typename T>
    static void removeResource(const std::string &resource_name);

    template<typename T, typename... Args>
    static T *loadFromFile(const std::string &resource_name, Args &&...args);
    template<typename T, typename... Args>
    static T *loadFromMemory(const std::string &resource_name, Args &&...args);
    template<typename T, typename... Args>
    static T *loadFromStream(const std::string &resource_name, Args &&...args);

private:
    ResourceCache() = default;
    ~ResourceCache() = default;

    static ResourceCache &instance();

    struct CacheBase
    {
    };

    template<typename T>
    struct Cache : public CacheBase
    {
        std::unordered_map<std::string, std::unique_ptr<T>> data;
    };

    template<typename T>
    Cache<T> *cache(bool need_create = true);

private:
    std::unordered_map<std::type_index, std::unique_ptr<CacheBase>> m_caches;
};

template<typename T>
T *ResourceCache::resource(const std::string &resource_name)
{
    Cache<T> *c = instance().cache<T>(false);
    if (!c)
        return nullptr;

    auto found = c->data.find(resource_name);
    if (found == c->data.end())
        return nullptr;

    return found->second.get();
}

template<typename T>
void ResourceCache::addResource(const std::string &resource_name, T *resource)
{
    std::shared_ptr<Cache<T>> c = instance().cache<T>();
    c->data.emplace(resource_name, std::unique_ptr<T>(resource));
}

template<typename T>
void ResourceCache::removeResource(const std::string &resource_name)
{
    std::type_index type_index = typeid(T);
    auto cache_found = instance().m_caches.find(type_index);
    if (cache_found != instance().m_caches.end())
    {
        Cache<T> *cache = static_cast<Cache<T> *>(cache_found->second.get());
        auto resource_found = cache->data.find(resource_name);
        if (resource_found != cache->data.end())
            cache->data.erase(resource_found);
    }
}

template<typename T, typename... Args>
T *ResourceCache::loadFromFile(const std::string &resource_name, Args &&...args)
{
    T *exist_resource = resource<T>(resource_name);
    if (exist_resource)
        return exist_resource;

    std::unique_ptr<T> res = std::make_unique<T>();
    T *res_ptr = res.get();
    if (!res->loadFromFile(std::forward<Args>(args)...))
        return nullptr;

    Cache<T> *c = instance().cache<T>();
    c->data.emplace(resource_name, std::move(res));

    spdlog::debug("Load resource: \"{0}\"", resource_name);

    return res_ptr;
}

template<typename T, typename... Args>
T *ResourceCache::loadFromMemory(const std::string &resource_name, Args &&...args)
{
    T *exist_resource = resource<T>(resource_name);
    if (exist_resource)
        return exist_resource;

    std::unique_ptr<T> res = std::make_unique<T>();
    T *res_ptr = res.get();
    if (!res->loadFromMemory(std::forward<Args>(args)...))
        return nullptr;

    Cache<T> *c = instance().cache<T>();
    c->data.emplace(resource_name, std::move(res));

    spdlog::debug("Load resource: \"{0}\"", resource_name);

    return res_ptr;
}

template<typename T, typename... Args>
T *ResourceCache::loadFromStream(const std::string &resource_name, Args &&...args)
{
    T *exist_resource = resource<T>(resource_name);
    if (exist_resource)
        return exist_resource;

    std::unique_ptr<T> res = std::make_unique<T>();
    T *res_ptr = res.get();
    if (!res->loadFromStream(std::forward<Args>(args)...))
        return nullptr;

    Cache<T> *c = instance().cache<T>();
    c->data.emplace(resource_name, std::move(res));

    spdlog::debug("Load resource: \"{0\"}", resource_name);

    return res_ptr;
}

template<typename T>
ResourceCache::Cache<T> *ResourceCache::cache(bool need_create)
{
    std::type_index type_index = typeid(T);
    Cache<T> *cache = nullptr;
    auto found = m_caches.find(type_index);
    if (found != m_caches.end())
    {
        cache = static_cast<ResourceCache::Cache<T> *>(found->second.get());
    }
    else if (need_create)
    {
        cache = new Cache<T>();
        m_caches.emplace(type_index, std::unique_ptr<CacheBase>(cache));
    }
    return cache;
}

} // namespace fck

#endif // RESOURCECACHE_H
