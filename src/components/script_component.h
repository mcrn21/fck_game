#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "../entity_script_base.h"
#include "../knowledge_base.h"

#include <memory>

namespace fck
{

struct ScriptComponent
{
    std::unique_ptr<EntityScriptBase> entity_script;
};

template<>
struct KnowledgeBase::ComponentItem<ScriptComponent> : ComponentItemBase
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
        ScriptComponent &component = entity.addComponent<ScriptComponent>();

        component.entity_script.reset(KnowledgeBase::createEntityScript(entity_script));
    }

    std::string entity_script;
};

KNOWLEDGE_BASE_REGISTER_COMPONENT(ScriptComponent);

} // namespace fck

#endif // SCRIPTCOMPONENT_H
