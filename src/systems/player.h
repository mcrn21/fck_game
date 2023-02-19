#ifndef PLAYER_IGIYBAQERLJC_H
#define PLAYER_IGIYBAQERLJC_H

#include "../components/components.h"
#include "../fck/system.h"
#include "../fck_common.h"

namespace fck::system
{

class Player : public System<component::Player, component::Velocity>
{
public:
    enum MoveDirection
    {
        LEFT = 1,
        UP = 2,
        RIGHT = 4,
        DOWN = 8
    };

    Player();
    ~Player() = default;

    void update(double delta_time);

public: // slots
    void onActionActivated(keyboard_action::Action action);
    void onActionDiactivated(keyboard_action::Action action);

    void onEntityTargetChanged(
        const Entity &entity, const Entity &target, const Entity &old_target);

private:
    int32_t m_move_direction;
    bool m_need_update_state;
};

} // namespace fck::system

#endif // PLAYERACTIONS_IGIYBAQERLJC_H
