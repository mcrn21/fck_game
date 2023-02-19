#ifndef SCRIPTFACTORY_UNZYSWTLGSSI_H
#define SCRIPTFACTORY_UNZYSWTLGSSI_H

#include "../scripts/script.h"

#include <sol/sol.hpp>

namespace fck
{

class ScriptFactory
{
public:
    struct Factory
    {
        script::Script *createScript();
        sol::protected_function create_function;
    };

    static void setSolState(sol::state *sol_state);
    static void registerScriptFacory(const std::string &script_name, const std::string &script_str);

    static script::Script *createScript(const std::string &script_name);

private:
    static ScriptFactory &instance();

    ScriptFactory();
    ~ScriptFactory() = default;

private:
    sol::state *m_sol_state;
    std::unordered_map<std::string, std::unique_ptr<Factory>> m_script_factories;
};

} // namespace fck

#endif // SCRIPTFACTORY_UNZYSWTLGSSI_H
