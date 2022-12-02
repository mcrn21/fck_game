#ifndef PLAYERACTIONSSYSTEM_H
#define PLAYERACTIONSSYSTEM_H

#include "../fck_common.h"

#include "../components/components.h"

#include "../fck/input_actions_map.h"
#include "../fck/system.h"

namespace fck
{

class PlayerActionsSystem : public System<PlayerComponent>
{
public:
    PlayerActionsSystem();
    ~PlayerActionsSystem() = default;

    void setInputActions(InputActionsMap<keyboard_action::Action> *input_actions);

    void update(double delta_time);

public: // slots
    void onActionActivated(keyboard_action::Action action);

private:
    InputActionsMap<keyboard_action::Action> *m_input_actions;
    Id m_action_activated_conenction;
};

} // namespace fck

#endif // PLAYERACTIONSSYSTEM_H
