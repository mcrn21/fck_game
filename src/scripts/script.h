#ifndef SCRIP_ETJNVFCVLHHO_H
#define SCRIP_ETJNVFCVLHHO_H

#include "../fck/entity.h"
#include "../fck_common.h"

#include <sol/sol.hpp>

#include <SFML/Graphics.hpp>

namespace fck::script
{

class Script
{
public:
    Script(const sol::table &script_table);
    ~Script() = default;

    sol::table &getScriptTable();
    void setEntityToTable(const Entity &entity);

    void update(double delta_time);

    void onEntityEnabled();
    void onEntityDisabled();
    void onEntityDestroyed();

    void onEntityMoved(const sf::Vector2f &offset);

    void onEntityStateChanged(entity_state::State state);
    void onEntityDirectionChanged(entity_state::Direction direction);

    void onEntityCollided(const Entity &other);

private:
    sol::table m_script_table;

    sol::protected_function m_update_function;

    sol::protected_function m_entity_enabled_function;
    sol::protected_function m_entity_disabled_function;
    sol::protected_function m_entity_destroyed_function;

    sol::protected_function m_entity_moved_function;

    sol::protected_function m_entity_state_changed_function;
    sol::protected_function m_entity_direction_changed_function;

    sol::protected_function m_entity_collided_function;
};

} // namespace fck::script

#endif // SCRIP_ETJNVFCVLHHO_H
