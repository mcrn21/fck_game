#include "../fck/event_dispatcher.h"
#include "../fck/id_storage.h"
#include "bindings.h"

namespace fck
{

void bindFckToLua(sol::state &sol_state)
{
    sol::table fck_namespace = sol_state["fck"].get_or_create<sol::table>();

    // Id
    sol::usertype<Id> id_bind = fck_namespace.new_usertype<Id>(
        "Id",
        sol::constructors<Id(), Id(uint64_t), Id(uint64_t, uint64_t)>(),
        "isValid",
        &Id::operator bool,
        "getId",
        &Id::getId,
        "getIndex",
        &Id::getIndex,
        "getVersion",
        &Id::getVersion,
        sol::meta_function::equal_to,
        &Id::operator==);

    // IdStorage
    sol::usertype<IdStorage> id_storafe_bind = fck_namespace.new_usertype<IdStorage>(
        "IdStorage",
        sol::constructors<IdStorage(), IdStorage(int32_t)>(),
        "create",
        &IdStorage::create,
        "destroy",
        &IdStorage::destroy,
        "get",
        &IdStorage::get,
        "isValid",
        &IdStorage::isValid,
        "resize",
        &IdStorage::resize,
        "clear",
        &IdStorage::clear,
        "size",
        &IdStorage::size);

    // EventDispatcher
    sol::table event_dispatcher_namespace
        = fck_namespace["EventDispatcher"].get_or_create<sol::table>();

    event_dispatcher_namespace["runTask"] = [](const sol::object &obj, const sol::object &func) {
        EventDispatcher::runTask([func]() {
            sol::protected_function task = func.as<sol::protected_function>();
            auto result = task();
            if (!result.valid())
            {
                sol::error err = result;
                spdlog::warn("Error at EventDispatcher::runTask: {}", err.what());
            }
        });
    };
    event_dispatcher_namespace["runTasks"] = [](const sol::object &obj, const sol::object &funcs) {
        std::vector<sol::object> objects = funcs.as<std::vector<sol::object>>();
        std::vector<std::function<bool()>> tasks;
        for (const sol::object &o : objects)
        {
            std::function<bool()> task = o.as<sol::function>();
            tasks.push_back([o]() -> bool {
                sol::protected_function task = o.as<sol::protected_function>();
                auto result = task();
                if (!result.valid())
                {
                    sol::error err = result;
                    spdlog::warn("Error at EventDispatcher::runTasks: {}", err.what());
                    return false;
                }
                else
                {
                    return result;
                }
            });
        }
        EventDispatcher::runTasks(tasks);
    };
}

sol::usertype<Entity> bindEntityToLua(sol::state &sol_state)
{
    sol::table fck_namespace = sol_state["fck"].get_or_create<sol::table>();

    sol::usertype<Entity> entity_bind = fck_namespace.new_usertype<Entity>(
        "Entity",
        sol::constructors<Entity()>(),
        "getId",
        &Entity::getId,
        //        "getWorld",
        //        &Entity::getWorld,
        "isValid",
        &Entity::isValid,
        "isEnabled",
        &Entity::isEnabled,
        "enable",
        &Entity::enable,
        "disable",
        &Entity::disable,
        "destroy",
        &Entity::destroy);

    return entity_bind;
}

} // namespace fck
