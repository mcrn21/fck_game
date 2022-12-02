#ifndef TESTACTION_H
#define TESTACTION_H

#include "../action_base.h"

#include <SFML/System/Vector2.hpp>

namespace fck
{

class TestActionResult : public ActionResultBase
{
public:
    TestActionResult(const Entity &entity, const std::vector<Entity> &targes);
    ~TestActionResult() = default;

    bool update(double delta_time);

private:
    Entity m_entity;
    std::vector<Entity> m_targets;

    double m_interval;
    double m_max_interval;

    bool m_first_update;

    bool m_damaged;
    sf::Vector2f m_point;
};

class TestAction : public ActionBase
{
public:
    TestAction();
    ~TestAction() = default;

protected:
    ActionResultBase *createResult(const Entity &entity, const std::vector<Entity> &targets);
};

} // namespace fck

#endif // TESTACTION_H
