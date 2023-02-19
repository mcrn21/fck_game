#include "entity_factory.h"
#include "../components/components.h"
#include "../fck/utilities.h"
#include "../fck/world.h"
#include "knowledge_base.h"

namespace fck
{

EntityFactory::Factory::Factory(Factory &&other)
{
    m_components = std::move(other.m_components);
}

EntityFactory::Factory &EntityFactory::Factory::operator=(Factory &&other)
{
    m_components = std::move(other.m_components);
    return *this;
}

bool EntityFactory::Factory::init(toml::table &table)
{
    m_components.clear();

    try
    {
        toml::table *components_table = table.at("components").as_table();

        for (auto &it : *components_table)
        {
            if (!it.second.is_table())
                continue;

            toml::table *component_table = it.second.as_table();

            component_type::Type component_type = component_type::fromString(it.first.data());
            if (component_type == component_type::NO_TYPE)
                throw Exception(fmt::format("Component type not found: {}", it.first.data()));

            ComponentFactory::Factory *component_factory
                = ComponentFactory::createComponentFactory(component_type);
            if (!component_factory)
                throw Exception(
                    fmt::format("Can't create component factory for type: ", it.first.data()));

            component_factory->component_table = *component_table;
            m_components.push_back(std::unique_ptr<ComponentFactory::Factory>(component_factory));
        }
    }
    catch (const std::exception &e)
    {
        spdlog::warn(e.what());
        return false;
    }

    return true;
}

Entity EntityFactory::Factory::createEntity(World *world)
{
    Entity entity = world->createEntity();

    try
    {
        for (const auto &it : m_components)
            it->create(entity);
    }
    catch (const std::exception &e)
    {
        spdlog::warn(e.what());
        entity.destroy();
        entity = Entity{};
    }

    return entity;
}

void EntityFactory::registerEntityFactory(const std::string &entity_name, Factory &&entity_factory)
{
    spdlog::info("Register entity factory: {}", entity_name);
    instance().m_entity_factories[entity_name]
        = std::make_unique<Factory>(std::move(entity_factory));
}

void EntityFactory::registerEntityFactory(const std::string &entity_name, toml::table &table)
{
    Factory entity_factory;

    if (!entity_factory.init(table))
    {
        spdlog::warn("Can't create entity factory for: {}", entity_name);
        return;
    }

    registerEntityFactory(entity_name, std::move(entity_factory));
}

void EntityFactory::registerEntityFactory(
    const std::string &entity_name, const std::string &table_str)
{
    toml::table table = toml::parse(table_str);
    EntityFactory::registerEntityFactory(entity_name, table);
}

Entity EntityFactory::createEntity(const std::string &entity_name, World *world)
{
    if (!world)
        return {};

    auto entities_found = instance().m_entity_factories.find(entity_name);
    if (entities_found == instance().m_entity_factories.end())
        return Entity{};

    return entities_found->second->createEntity(world);
}

Entity EntityFactory::createPlayer(const std::string &entity_name, World *world)
{
    Entity entity = createEntity(entity_name, world);

    if (entity.isValid())
        entity.add<component::Player>();

    return entity;
}

EntityFactory &EntityFactory::instance()
{
    static EntityFactory entities_factory;
    return entities_factory;
}

} // namespace fck
