#include "resource_cache.h"

namespace fck
{

ResourceCache &ResourceCache::instance()
{
    static ResourceCache resource_cache;
    return resource_cache;
}

} // namespace fck
