#ifndef CLASSTYPEID_H
#define CLASSTYPEID_H

#include <cstdint>

namespace fck
{

typedef uint64_t TypeId;

template<typename T>
class ClassTypeId
{
public:
    template<typename U>
    static TypeId id()
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

#endif // CLASSTYPEID_H
