#include "chunk_entry.h"

#include "../components/components.h"
#include "../entity_utils.h"
#include "../fck/event_dispatcher.h"

namespace fck::entity_script
{

ChunkEntry::ChunkEntry(map::Map *map, chunk_side::Side side)
    : m_map{map}, m_side{side}, m_used{false}
{
}

void ChunkEntry::onEntityEnabled(const Entity &entity)
{
    m_used = false;
}

void ChunkEntry::onEntityCollided(const Entity &entity, const Entity &other)
{
    if (m_used || !other.isValid() || !other.has<component::Player>())
        return;

    static std::unordered_map<chunk_side::Side, sf::Vector2i> neighbor_chunks
        = {{chunk_side::LEFT, {-1, 0}},
           {chunk_side::RIGHT, {1, 0}},
           {chunk_side::TOP, {0, -1}},
           {chunk_side::BOTTOM, {0, 1}}};

    static std::unordered_map<chunk_side::Side, chunk_side::Side> opposite_sides
        = {{chunk_side::LEFT, chunk_side::RIGHT},
           {chunk_side::RIGHT, chunk_side::LEFT},
           {chunk_side::TOP, chunk_side::BOTTOM},
           {chunk_side::BOTTOM, chunk_side::TOP}};

    m_used = true;

    sf::Vector2i new_chunk_coords = m_map->getCurrentChunk() + neighbor_chunks[m_side];
    sf::Vector2f other_position;

    auto other_scene_component = other.get<component::Scene>();

    Entity new_chunk_entry_entity = m_map->getChunks()
                                        .getData(new_chunk_coords)
                                        ->getChunkEntityEntities()
                                        .at(opposite_sides[m_side]);
    auto new_chunk_entry_entity_scene_component = new_chunk_entry_entity.get<component::Scene>();
    auto new_chunk_entry_entity_transform_component
        = new_chunk_entry_entity.get<component::Transform>();

    other_position
        = {new_chunk_entry_entity_transform_component.transform.getPosition().x
               + new_chunk_entry_entity_scene_component.local_bounds.width / 2,
           new_chunk_entry_entity_transform_component.transform.getPosition().y
               + new_chunk_entry_entity_scene_component.local_bounds.height / 2};

    switch (m_side)
    {
    case chunk_side::LEFT:
        other_position.x
            -= (other_scene_component.local_bounds.width
                + new_chunk_entry_entity_scene_component.local_bounds.width / 2);
        break;
    case chunk_side::RIGHT:
        other_position.x
            += (other_scene_component.local_bounds.width
                + new_chunk_entry_entity_scene_component.local_bounds.width / 2);
        break;
    case chunk_side::TOP:
        other_position.y
            -= (other_scene_component.local_bounds.height
                + new_chunk_entry_entity_scene_component.local_bounds.height / 2);
        break;
    case chunk_side::BOTTOM:
        other_position.y
            += (other_scene_component.local_bounds.height
                + new_chunk_entry_entity_scene_component.local_bounds.height / 2);
        break;
    default:
        break;
    }

    EventDispatcher::runTask([this, other_position, new_chunk_coords, other]() {
        m_map->setCurrentChunk(new_chunk_coords, {other});

        entity::set_position.emit(other, other_position);

        auto &velocity_component = other.get<component::Velocity>();
        velocity_component.velocity = {};

        auto &player_component = other.get<component::Player>();
        player_component.view_hard_set_position = true;

        m_used = false;
    });
}

} // namespace fck::entity_script
