#include "id_storage.h"

namespace fck
{

Id Id::invalid;

IdStorage::IdStorage(int32_t size) : m_next_index{0}, m_versions(size)
{
}

Id IdStorage::create()
{
    uint32_t index = 0;
    if (!m_free_indexes.empty())
    {
        index = *m_free_indexes.begin();
        m_free_indexes.erase(m_free_indexes.begin());
    }
    else
    {
        index = m_next_index++;
        m_versions.resize(index + 1, 1);
    }

    return Id(index, m_versions[index]);
}

void IdStorage::destroy(const Id &id)
{
    if (!isValid(id))
        return;

    ++m_versions[id];
    m_free_indexes.insert(id.index());
}

Id IdStorage::get(uint32_t index) const
{
    if (index >= m_versions.size())
        return Id::invalid;
    return Id(index, m_versions[index]);
}

bool IdStorage::isValid(const Id &id) const
{
    return id.index() < m_next_index && id.version() == m_versions[id];
}

void IdStorage::resize(int32_t size)
{
    m_versions.resize(size);
}

void IdStorage::clear()
{
    m_next_index = 0;
    m_free_indexes.clear();
    m_versions.clear();
}

int32_t IdStorage::size() const
{
    return m_versions.size();
}

} // namespace fck
