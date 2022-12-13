#ifndef COMPONENT_VRNBCMXRKQFI_H
#define COMPONENT_VRNBCMXRKQFI_H

#include "class_type_id.h"

#include <cstdint>
#include <memory>

namespace fck
{

const int32_t MAX_AMOUNT_OF_COMPONENTS = 64;

struct ComponentBase
{
    virtual ~ComponentBase() = default;
};

template<typename T>
struct Component : public ComponentBase
{
    Component(T *t_data) : data{t_data}
    {
    }
    std::unique_ptr<T> data;
};

template<class T>
TypeId componentTypeId()
{
    return ClassTypeId<ComponentBase>::getId<T>();
}

struct ComponentsFilter
{
    template<typename... Args>
    static ComponentsFilter create();
    void clear()
    {
        filter = 0;
    }
    bool test(const ComponentsFilter &componentFilter)
    {
        return (filter & componentFilter.filter) == componentFilter.filter;
    }
    uint64_t filter = 0;
};

template<typename... Args>
ComponentsFilter ComponentsFilter::create()
{
    ComponentsFilter m;
    using expander = int[];
    (void)expander{0, (void(m.filter |= (1 << componentTypeId<Args>())), 0)...};
    return m;
}

} // namespace fck

#endif // COMPONENT_VRNBCMXRKQFI_H
