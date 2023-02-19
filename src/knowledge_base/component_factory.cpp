#include "component_factory.h"

namespace fck
{

ComponentFactory::Factory *ComponentFactory::createComponentFactory(component_type::Type type)
{
    auto component_factories_found = instance().m_component_factories.find(type);
    if (component_factories_found == instance().m_component_factories.end())
        return nullptr;
    return component_factories_found->second();
}

ComponentFactory &ComponentFactory::instance()
{
    static ComponentFactory components_factory;
    return components_factory;
}

} // namespace fck
