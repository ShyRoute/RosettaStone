// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Actions/Generic.hpp>
#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Tasks/SimpleTasks/DamageNumberTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/IncludeTask.hpp>

namespace RosettaStone::SimpleTasks
{
DamageNumberTask::DamageNumberTask(EntityType entityType, bool isSpellDamage)
    : ITask(entityType), m_isSpellDamage(isSpellDamage)
{
    // Do nothing
}

TaskID DamageNumberTask::GetTaskID() const
{
    return TaskID::DAMAGE_NUMBER;
}

TaskStatus DamageNumberTask::Impl(Player& player)
{
    const int damage = m_source->owner->GetGame()->taskStack.num;
    auto entities =
        IncludeTask::GetEntities(m_entityType, player, m_source, m_target);

    for (auto& entity : entities)
    {
        const auto character = dynamic_cast<Character*>(entity);
        Generic::TakeDamageToCharacter(m_source, character, damage,
                                       m_isSpellDamage);
    }

    return TaskStatus::COMPLETE;
}

ITask* DamageNumberTask::CloneImpl()
{
    return new DamageNumberTask(m_entityType, m_isSpellDamage);
}
}  // namespace RosettaStone::SimpleTasks
