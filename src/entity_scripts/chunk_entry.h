#ifndef CHUNKENTRY_UMLMUIYPVRHX_H
#define CHUNKENTRY_UMLMUIYPVRHX_H

#include "entity_script_base.h"

#include "../fck_common.h"
#include "../map/map.h"

namespace fck::entity_script
{

class ChunkEntry : public EntityScriptBase
{
public:
    ChunkEntry(map::Map *map, chunk_side::Side side);
    ~ChunkEntry() = default;

    void onEntityEnabled(const Entity &entity);
    void onEntityCollided(const Entity &entity, const Entity &other);

private:
    map::Map *m_map;
    chunk_side::Side m_side;

    bool m_used;
};

} // namespace fck::entity_script

#endif // CHUNKENTRY_UMLMUIYPVRHX_H
