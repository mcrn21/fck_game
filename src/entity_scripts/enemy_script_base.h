#ifndef ENEMYSCRIPTBASE_H
#define ENEMYSCRIPTBASE_H

#include "../entity_script_base.h"
#include "../knowledge_base.h"

#include "toml++/toml.h"

namespace fck
{

class EnemyScriptBase : public EntityScriptBase
{
public:
    EnemyScriptBase(double attack_interval);
    ~EnemyScriptBase() = default;

    void update(const Entity &entity, double delta_time);

private:
    double m_attack_interval;
};

template<>
struct KnowledgeBase::EntityScriptItem<EnemyScriptBase> : EntityScriptItemBase
{
    const std::string &name() const
    {
        return m_name;
    }

    EntityScriptBase *create() const
    {
        return new EnemyScriptBase{m_attack_interval};
    }

private:
    void init(toml::table *table)
    {
        m_name = table->at("name").as_string()->get();

        m_attack_interval = table->at("attack_interval").as_floating_point()->get();
    }

private:
    std::string m_name;

    double m_attack_interval = 0.5;
};

KNOWLEDGE_BASE_REGISTER_BASE_ENTITY_SCRIPT(EnemyScriptBase, "enemy_script_base");

} // namespace fck

#endif // ENEMYSCRIPTBASE_H
