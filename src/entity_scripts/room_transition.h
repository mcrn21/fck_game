#ifndef ROOMTRANSITION_FHHISUHQXNKH_H
#define ROOMTRANSITION_FHHISUHQXNKH_H

#include "../entity_script_base.h"
#include "../knowledge_base.h"
#include "../level.h"

#include "toml++/toml.h"

namespace fck::entity_script
{

class RoomTransition : public EntityScriptBase
{
public:
    RoomTransition(Level *level);
    ~RoomTransition() = default;

    void setSide(Room::Side side);
    void setRoomCoord(const sf::Vector2i &room_coord);

    void update(const Entity &entity, double delta_time);

    void onEntityEnabled(const Entity &entity);
    void onEntityCollided(const Entity &entity, const Entity &other);

private:
    Level *m_level;

    Room::Side m_side;
    sf::Vector2i m_room_coord;

    bool m_need_change_room;
    bool m_used;
};

} // namespace fck::entity_script

#endif // ROOMTRANSITION_FHHISUHQXNKH_H
