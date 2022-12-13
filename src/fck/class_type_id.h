#ifndef CLASSTYPEID_UJPWZHTGLAZP_H
#define CLASSTYPEID_UJPWZHTGLAZP_H

#include <cstdint>

namespace fck
{

typedef uint64_t TypeId;

template<typename T>
class ClassTypeId
{
public:
    template<typename U>
    static TypeId getId()
    {
        static const TypeId id = m_next++;
        return id;
    }

private:
    static uint64_t m_next;
};

template<typename T>
TypeId ClassTypeId<T>::m_next{0};

} // namespace fck

#endif // CLASSTYPEID_UJPWZHTGLAZP_H
