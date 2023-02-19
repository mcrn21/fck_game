#include "drawable_factory.h"

namespace fck
{

std::tuple<DrawableProxyBase *, DrawableState *, DrawableAnimation *>
DrawableFactory::createDrawable(drawable_type::Type drawable_type, toml::table *table)
{
    auto loader_found = instance().m_loaders.find(drawable_type);
    if (loader_found == instance().m_loaders.end())
        return {nullptr, nullptr, nullptr};
    return loader_found->second->fromToml(table);
}

DrawableFactory &DrawableFactory::instance()
{
    static DrawableFactory drawable_loader;
    return drawable_loader;
}

} // namespace fck
