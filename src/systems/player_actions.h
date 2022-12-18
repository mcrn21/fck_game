#ifndef PLAYERACTIONS_IGIYBAQERLJC_H
#define PLAYERACTIONS_IGIYBAQERLJC_H

#include "../fck_common.h"

#include "../components/components.h"

#include "../fck/system.h"

namespace fck::system
{

class PlayerActions : public System<component::Player, component::Velocity>
{
public:
    enum MoveDirection
    {
        LEFT = 1,
        UP = 2,
        RIGHT = 4,
        DOWN = 8
    };

    PlayerActions();
    ~PlayerActions() = default;

    void update(double delta_time);

public: // slots
    void onActionActivated(keyboard_action::Action action);
    void onActionDiactivated(keyboard_action::Action action);

private:
    int32_t m_move_direction;
    bool m_need_update_state;
};

} // namespace fck::system

#endif // PLAYERACTIONS_IGIYBAQERLJC_H
