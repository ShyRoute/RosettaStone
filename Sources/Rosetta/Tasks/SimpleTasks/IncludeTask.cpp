// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Tasks/SimpleTasks/IncludeTask.hpp>

#include <stdexcept>
#include <utility>

namespace RosettaStone::SimpleTasks
{
IncludeTask::IncludeTask(EntityType entityType,
                         std::vector<EntityType> excludeTypes)
    : ITask(entityType), m_excludeTypes(std::move(excludeTypes))
{
    // Do nothing
}

TaskID IncludeTask::GetTaskID() const
{
    return TaskID::INCLUDE;
}

std::vector<Entity*> IncludeTask::GetEntities(EntityType entityType,
                                              Player& player, Entity* source,
                                              Entity* target)
{
    std::vector<Entity*> entities;

    switch (entityType)
    {
        case EntityType::SOURCE:
            if (source != nullptr)
            {
                entities.emplace_back(source);
            }
            break;
        case EntityType::TARGET:
            if (target != nullptr)
            {
                entities.emplace_back(target);
            }
            break;
        case EntityType::ALL:
            for (auto& minion : player.GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            entities.emplace_back(player.GetHero());
            for (auto& minion : player.opponent->GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            entities.emplace_back(player.opponent->GetHero());
            break;
        case EntityType::ALL_NOSOURCE:
            for (auto& minion : player.GetFieldZone().GetAll())
            {
                if (source == minion)
                {
                    continue;
                }
                entities.emplace_back(minion);
            }
            entities.emplace_back(player.GetHero());
            for (auto& minion : player.opponent->GetFieldZone().GetAll())
            {
                if (source == minion)
                {
                    continue;
                }
                entities.emplace_back(minion);
            }
            entities.emplace_back(player.opponent->GetHero());
            break;
        case EntityType::FRIENDS:
            for (auto& minion : player.GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            entities.emplace_back(player.GetHero());
            break;
        case EntityType::ENEMIES:
            for (auto& minion : player.opponent->GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            entities.emplace_back(player.opponent->GetHero());
            break;
        case EntityType::ENEMIES_NOTARGET:
            if (target == player.opponent->GetHero())
            {
                for (auto& minion : player.opponent->GetFieldZone().GetAll())
                {
                    entities.emplace_back(minion);
                }
            }
            else
            {
                for (auto& minion : player.opponent->GetFieldZone().GetAll())
                {
                    if (target == minion)
                    {
                        continue;
                    }

                    entities.emplace_back(minion);
                }

                entities.emplace_back(player.opponent->GetHero());
            }
            break;
        case EntityType::HERO:
            entities.emplace_back(player.GetHero());
            break;
        case EntityType::ENEMY_HERO:
            entities.emplace_back(player.opponent->GetHero());
            break;
        case EntityType::WEAPON:
            if (player.GetHero()->weapon != nullptr)
            {
                entities.emplace_back(player.GetHero()->weapon);
            }
            break;
        case EntityType::ENEMY_WEAPON:
            if (player.opponent->GetHero()->weapon != nullptr)
            {
                entities.emplace_back(player.opponent->GetHero()->weapon);
            }
            break;
        case EntityType::HAND:
            for (auto& card : player.GetHandZone().GetAll())
            {
                entities.emplace_back(card);
            }
            break;
        case EntityType::ENEMY_HAND:
            for (auto& card : player.opponent->GetHandZone().GetAll())
            {
                entities.emplace_back(card);
            }
            break;
        case EntityType::DECK:
            for (auto& card : player.GetDeckZone().GetAll())
            {
                entities.emplace_back(card);
            }
            break;
        case EntityType::ENEMY_DECK:
            for (auto& card : player.opponent->GetDeckZone().GetAll())
            {
                entities.emplace_back(card);
            }
            break;
        case EntityType::ALL_MINIONS:
            for (auto& minion : player.GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            for (auto& minion : player.opponent->GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            break;
        case EntityType::ALL_MINIONS_NOSOURCE:
            for (auto& minion : player.GetFieldZone().GetAll())
            {
                if (source == minion)
                {
                    continue;
                }

                entities.emplace_back(minion);
            }
            for (auto& minion : player.opponent->GetFieldZone().GetAll())
            {
                if (source == minion)
                {
                    continue;
                }

                entities.emplace_back(minion);
            }
            break;
        case EntityType::MINIONS:
            for (auto& minion : player.GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            break;
        case EntityType::MINIONS_NOSOURCE:
            for (auto& minion : player.GetFieldZone().GetAll())
            {
                if (source == minion)
                {
                    continue;
                }

                entities.emplace_back(minion);
            }
            break;
        case EntityType::ENEMY_MINIONS:
            for (auto& minion : player.opponent->GetFieldZone().GetAll())
            {
                entities.emplace_back(minion);
            }
            break;
        case EntityType::STACK:
            entities = player.GetGame()->taskStack.entities;
            break;
        default:
            throw std::invalid_argument(
                "IncludeTask::GetEntities() - Invalid entity type");
    }

    return entities;
}

TaskStatus IncludeTask::Impl(Player& player)
{
    const auto entities = GetEntities(m_entityType, player, m_source, m_target);

    if (!m_excludeTypes.empty())
    {
        std::vector<Entity*> exceptEntities;
        for (auto& excludeType : m_excludeTypes)
        {
            auto temp = GetEntities(excludeType, player, m_source, m_target);
            exceptEntities.insert(exceptEntities.end(), temp.begin(),
                                  temp.end());
        }

        std::vector<Entity*> result = entities;
        EraseIf(result, [&](Entity* entity) {
            for (auto& excludeEntity : exceptEntities)
            {
                if (entity == excludeEntity)
                {
                    return true;
                }
            }

            return false;
        });

        player.GetGame()->taskStack.entities = result;
    }
    else
    {
        player.GetGame()->taskStack.entities = entities;
    }

    return TaskStatus::COMPLETE;
}

ITask* IncludeTask::CloneImpl()
{
    return new IncludeTask(m_entityType, m_excludeTypes);
}
}  // namespace RosettaStone::SimpleTasks
