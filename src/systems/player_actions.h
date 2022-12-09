#ifndef PLAYERACTIONS_IGIYBAQERLJC_H
#define PLAYERACTIONS_IGIYBAQERLJC_H

#include "../fck_common.h"

#include "../components/components.h"

#include "../fck/input_actions_map.h"
#include "../fck/system.h"

namespace fck::system
{

class PlayerActions : public System<component::Player>
{
public:
    PlayerActions();
    ~PlayerActions() = default;

    void setInputActions(InputActionsMap<keyboard_action::Action> *input_actions);

    void update(double delta_time);

public: // slots
    void onActionActivated(keyboard_action::Action action);

private:
    InputActionsMap<keyboard_action::Action> *m_input_actions;
    Id m_action_activated_conenction;
};

} // namespace fck::system

#endif // PLAYERACTIONS_IGIYBAQERLJC_H
