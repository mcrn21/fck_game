#include "script_factory.h"

#include <spdlog/spdlog.h>

namespace fck
{

script::Script *ScriptFactory::Factory::createScript()
{
    auto result = create_function();
    if (!result.valid())
    {
        sol::error err = result;
        spdlog::warn("Create script error: {}", err.what());
        return nullptr;
    }

    sol::table script_table = result;
    return new script::Script{script_table};
}

void ScriptFactory::setSolState(sol::state *sol_state)
{
    instance().m_sol_state = sol_state;
}

void ScriptFactory::registerScriptFacory(
    const std::string &script_name, const std::string &script_str)
{
    instance().m_sol_state->script(script_str);

    std::unique_ptr<Factory> script_factory = std::make_unique<Factory>();
    script_factory->create_function = (*instance().m_sol_state)[script_name];

    instance().m_script_factories[script_name] = std::move(script_factory);

    spdlog::info("Register script factory: {}", script_name);
}

script::Script *ScriptFactory::createScript(const std::string &script_name)
{
    auto script_factories_found = instance().m_script_factories.find(script_name);
    if (script_factories_found == instance().m_script_factories.end())
        return nullptr;
    return script_factories_found->second->createScript();
}

ScriptFactory &ScriptFactory::instance()
{
    static ScriptFactory scripts_factory;
    return scripts_factory;
}

ScriptFactory::ScriptFactory() : m_sol_state{nullptr}
{
}

} // namespace fck
