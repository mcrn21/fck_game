#ifndef SIGSLOT_H
#define SIGSLOT_H

#include "id_storage.h"

#include <functional>
#include <utility>

namespace fck
{

template<typename... Args>
class Signal
{
public:
    Signal() = default;
    ~Signal() = default;

    template<typename T, typename Func>
    [[maybe_unused]] Id connect(T *obj, Func &&func)
    {
        auto f = [obj, func](Args... args) { (obj->*func)(std::forward<Args>(args)...); };
        Id id = m_id_storage.create();
        m_slots.emplace(id, f);
        return id;
    }

    [[maybe_unused]] Id connect(const std::function<void(Args...)> &slot)
    {
        Id id = m_id_storage.create();
        m_slots.emplace(id, slot);
        return id;
    }

    void disconnect(Id id)
    {
        auto slots_found = m_slots.find(id);
        if (slots_found != m_slots.end())
        {
            m_slots.erase(id);
            m_id_storage.destroy(id);
        }
    }
    void disconnect(const std::vector<Id> &ids)
    {
        for (const Id &id : ids)
            disconnect(id);
    }

    void disconnect()
    {
        for (auto it = m_slots.begin(); it != m_slots.end();)
        {
            m_id_storage.destroy(it->first);
            it = m_slots.erase(it);
        }
    }

    void emit(Args... args)
    {
        for (const auto &it : m_slots)
            it.second(std::forward<Args>(args)...);
    }

private:
    IdStorage m_id_storage;
    std::unordered_map<Id, std::function<void(Args...)>> m_slots;
};

} // namespace fck

#endif // SIGSLOT_H
