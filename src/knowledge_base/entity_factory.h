#ifndef ENTITYFACTORY_MNIXHSAOZOIA_H
#define ENTITYFACTORY_MNIXHSAOZOIA_H

#include "../fck/entity.h"
#include "../fck_common.h"
#include "component_factory.h"

#include <spdlog/spdlog.h>
#include <toml++/toml.h>

namespace fck
{

class EntityFactory
{
public:
    class Factory
    {
    public:
        Factory() = default;
        Factory(Factory &&other);

        Factory &operator=(Factory &&other);

        bool init(toml::table &table);
        Entity createEntity(World *world);

    private:
        std::vector<std::unique_ptr<ComponentFactory::Factory>> m_components;
    };

    static void registerEntityFactory(const std::string &entity_name, Factory &&entity_factory);
    static void registerEntityFactory(const std::string &entity_name, toml::table &table);
    static void registerEntityFactory(const std::string &entity_name, const std::string &table_str);

    static Entity createEntity(const std::string &entity_name, World *world);
    static Entity createPlayer(const std::string &entity_name, World *world);

private:
    static EntityFactory &instance();

    EntityFactory() = default;
    ~EntityFactory() = default;

private:
    std::unordered_map<std::string, std::unique_ptr<Factory>> m_entity_factories;
};

} // namespace fck

#endif // ENTITYFACTORY_MNIXHSAOZOIA_H
