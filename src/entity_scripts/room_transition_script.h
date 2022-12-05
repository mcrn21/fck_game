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

    const std::string room() const;
    void setRoom(const std::string &room);

    const sf::Vector2f &point() const;
    void setPoint(const sf::Vector2f &point);

    void update(const Entity &entity, double delta_time);

    void onEntityCollided(const Entity &entity, const Entity &other);

private:
    Level *m_level;

    std::string m_room;
    sf::Vector2f m_point;

    bool m_need_change_room;
    Entity m_transition_entity;
};

} // namespace fck

#endif // ROOMTRNASITIONSCRIPT_H
