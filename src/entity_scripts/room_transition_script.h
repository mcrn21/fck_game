#ifndef ROOMTRANSITIONSCRIPT_H
#define ROOMTRANSITIONSCRIPT_H

#include "../entity_script_base.h"
#include "../knowledge_base.h"
#include "../level.h"

#include "toml++/toml.h"

namespace fck
{

class RoomTransitionScript : public EntityScriptBase
{
public:
    RoomTransitionScript(Level *level);
    ~RoomTransitionScript() = default;

    void setRoomCoord(const sf::Vector2i &room_coord);
    void setTargetPosition(const sf::Vector2f &target_position);

    void update(const Entity &entity, double delta_time);

    void onEntityEnabled(const Entity &entity);

    void onEntityCollided(const Entity &entity, const Entity &other);

private:
    Level *m_level;

    sf::Vector2i m_room_coord;
    sf::Vector2f m_target_position;

    bool m_need_change_room;
    bool m_used;
};

} // namespace fck

#endif // ROOMTRNASITIONSCRIPT_H
