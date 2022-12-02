#ifndef ACTIONSSYSTEM_H
#define ACTIONSSYSTEM_H

#include "../action_base.h"

#include "../components/components.h"

#include "../fck/system.h"

#include <list>
#include <memory>

namespace fck
{

class ActionsSystem : public System<ActionsComponent>
{
public:
    ActionsSystem();
    ~ActionsSystem() = default;

    void update(double delta_time);

protected:
    void onEntityRemoved(Entity &entity);

private:
    void updateResults(double delta_time);
    void updateActions(double delta_time);

private:
    std::list<std::unique_ptr<ActionResultBase>> m_results;
    std::list<ActionBase *> m_cooldowning_actions;
};

} // namespace fck

#endif // ACTIONSSYSTEM_H
