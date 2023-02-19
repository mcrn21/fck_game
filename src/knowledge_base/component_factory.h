#ifndef COMPONENTFACTORY_HXPZLJLDRGLT_H
#define COMPONENTFACTORY_HXPZLJLDRGLT_H

#include "../fck/entity.h"
#include "../fck_common.h"

#include <spdlog/spdlog.h>
#include <toml++/toml.h>

namespace fck
{

#define REGISTER_COMPONENT_FACTORY(_type_, _class_) \
    inline const bool component_factory_##_class_ \
        = ::fck::ComponentFactory::registerComponentFactory<_class_>(_type_)

class ComponentFactory
{
public:
    struct Factory
    {
        virtual void create(Entity &entity) = 0;
        toml::table component_table;
    };

    template<typename T>
    static bool registerComponentFactory(component_type::Type type);
    static Factory *createComponentFactory(component_type::Type type);

private:
    static ComponentFactory &instance();

    ComponentFactory() = default;
    ~ComponentFactory() = default;

private:
    std::unordered_map<component_type::Type, std::function<Factory *()>> m_component_factories;
};

template<typename T>
bool ComponentFactory::registerComponentFactory(component_type::Type type)
{
    spdlog::info("Register component factory: {}", component_type::toString(type));
    instance().m_component_factories.emplace(type, []() { return new T{}; });
    return true;
}

} // namespace fck

#endif // COMPONENTFACTORY_HXPZLJLDRGLT_H
