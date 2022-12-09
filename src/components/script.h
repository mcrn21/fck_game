#ifndef SCRIPT_ADTFMKLTPCQP_H
#define SCRIPT_ADTFMKLTPCQP_H

#include "../entity_script_base.h"
#include "../knowledge_base.h"

#include <memory>

namespace fck
{

namespace component
{

struct Script
{
    std::unique_ptr<EntityScriptBase> entity_script;
};

} // namespace component

template<>
struct KnowledgeBase::ComponentItem<component::Script> : ComponentItemBase
{
    static component_type::Type componentType()
    {
        return component_type::SCRIPT;
    }

    void init(toml::table *table)
    {
        if (table->contains("entity_script"))
            entity_script = table->at("entity_script").as_string()->get();
    }

    void create(Entity &entity)
    {
        component::Script &component = entity.addComponent<component::Script>();

        component.entity_script.reset(KnowledgeBase::createEntityScript(entity_script));
    }

    std::string entity_script;
};

namespace component
{

KNOWLEDGE_BASE_REGISTER_COMPONENT(Script);

}

} // namespace fck

#endif // SCRIPTCOMPONENT_ADTFMKLTPCQP_H
