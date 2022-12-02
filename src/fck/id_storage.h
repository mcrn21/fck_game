#ifndef IDSTORAGE_H
#define IDSTORAGE_H

#include <spdlog/spdlog.h>

#include <stdint.h>
#include <bitset>
#include <functional>
#include <memory>
#include <set>
#include <vector>

namespace fck
{

struct Id
{
    static Id invalid;

    Id() : m_id(0)
    {
    }
    explicit Id(uint64_t id) : m_id(id)
    {
    }
    Id(uint32_t index, uint32_t version) : m_id(uint64_t(index) | uint64_t(version) << 32UL)
    {
    }
    Id(const Id &other) : m_id(other.m_id)
    {
    }

    bool operator==(const Id &other) const
    {
        return m_id == other.m_id;
    }

    bool operator!=(const Id &other) const
    {
        return m_id != other.m_id;
    }

    Id operator=(const Id &other)
    {
        m_id = other.m_id;
        return *this;
    }

    operator int32_t() const
    {
        return static_cast<int32_t>(m_id & 0xffffffffUL);
    }

    operator uint64_t() const
    {
        return static_cast<uint64_t>(m_id & 0xffffffffUL);
    }

    operator bool() const
    {
        return m_id != 0;
    }

    uint64_t id() const
    {
        return m_id;
    }
    uint32_t index() const
    {
        return m_id & 0xffffffffUL;
    }
    uint32_t version() const
    {
        return m_id >> 32;
    }

private:
    uint64_t m_id;
};

class IdStorage
{
public:
    IdStorage(int32_t size = 0);
    virtual ~IdStorage() = default;

    Id create();
    void destroy(const Id &id);
    Id get(uint32_t index) const;
    bool isValid(const Id &id) const;
    void resize(int32_t size);
    void clear();
    int32_t size() const;

private:
    uint32_t m_next_index;
    std::set<uint32_t> m_free_indexes;
    std::vector<uint32_t> m_versions;
};

} // namespace fck

namespace std
{

template<>
struct hash<::fck::Id>
{
    std::size_t operator()(const ::fck::Id &id) const
    {
        return std::hash<uint64_t>()(id.id());
    }
};

} // namespace std

#endif // IDSTORAGE_H
