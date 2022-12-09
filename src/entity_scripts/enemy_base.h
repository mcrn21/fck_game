#ifndef ENEMYBASE_PLHQVLCQKKCM_H
#define ENEMYBASE_PLHQVLCQKKCM_H

#include "../entity_script_base.h"
#include "../knowledge_base.h"

#include "toml++/toml.h"

namespace fck
{

namespace entity_script
{

class EnemyBase : public EntityScriptBase
{
public:
    EnemyBase(double attack_interval);
    ~EnemyBase() = default;

    void update(const Entity &entity, double delta_time);

private:
    double m_attack_interval;
};

} // namespace entity_script

template<>
struct KnowledgeBase::EntityScriptItem<entity_script::EnemyBase> : EntityScriptItemBase
{
    const std::string &name() const
    {
        return m_name;
    }

    EntityScriptBase *create() const
    {
        return new entity_script::EnemyBase{m_attack_interval};
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

namespace entity_script
{

KNOWLEDGE_BASE_REGISTER_BASE_ENTITY_SCRIPT(EnemyBase, "enemy_base");

}

} // namespace fck

#endif // ENEMYSCRIPTBASE_PLHQVLCQKKCM_H
