// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Conditions/SelfCondition.hpp>
#include <Rosetta/Games/Game.hpp>

#include <string>
#include <utility>

namespace RosettaStone
{
SelfCondition::SelfCondition(std::function<bool(Entity*)> func)
    : m_func(std::move(func))
{
    // Do nothing
}

SelfCondition SelfCondition::IsHeroPowerCard(const std::string& cardID)
{
    return SelfCondition([=](Entity* entity) -> bool {
        return entity->owner->GetHero()->heroPower->card->id == cardID;
    });
}

SelfCondition SelfCondition::IsDead()
{
    return SelfCondition(
        [=](Entity* entity) -> bool { return entity->isDestroyed; });
}

SelfCondition SelfCondition::IsNotDead()
{
    return SelfCondition(
        [=](Entity* entity) -> bool { return !entity->isDestroyed; });
}

SelfCondition SelfCondition::IsFieldFull()
{
    return SelfCondition([=](Entity* entity) -> bool {
        return entity->owner->GetFieldZone().IsFull();
    });
}

SelfCondition SelfCondition::IsDamaged()
{
    return SelfCondition([=](Entity* entity) -> bool {
        const auto character = dynamic_cast<Character*>(entity);
        if (!character)
        {
            return false;
        }

        return character->GetDamage() > 0;
    });
}

SelfCondition SelfCondition::IsUndamaged()
{
    return SelfCondition([=](Entity* entity) -> bool {
        const auto character = dynamic_cast<Character*>(entity);
        if (!character)
        {
            return false;
        }

        return character->GetDamage() == 0;
    });
}

SelfCondition SelfCondition::IsWeaponEquipped()
{
    return SelfCondition([=](Entity* entity) -> bool {
        return entity->owner->GetHero()->HasWeapon();
    });
}

SelfCondition SelfCondition::IsRace(Race race)
{
    return SelfCondition([=](Entity* entity) -> bool {
        return entity->card->GetRace() == race;
    });
}

SelfCondition SelfCondition::IsControllingRace(Race race)
{
    return SelfCondition([=](Entity* entity) -> bool {
        for (auto& minion : entity->owner->GetFieldZone().GetAll())
        {
            if (minion->card->GetRace() == race)
            {
                return true;
            }
        }

        return false;
    });
}

SelfCondition SelfCondition::IsControllingSecret()
{
    return SelfCondition([=](Entity* entity) -> bool {
        return !entity->owner->GetSecretZone().IsEmpty();
    });
}

SelfCondition SelfCondition::IsMinion()
{
    return SelfCondition([=](Entity* entity) -> bool {
        return dynamic_cast<Minion*>(entity) != nullptr;
    });
}

SelfCondition SelfCondition::IsSecret()
{
    return SelfCondition([=](Entity* entity) -> bool {
        return dynamic_cast<Spell*>(entity) != nullptr &&
               entity->GetGameTag(GameTag::SECRET) == 1;
    });
}

SelfCondition SelfCondition::IsFrozen()
{
    return SelfCondition([=](Entity* entity) -> bool {
        const auto character = dynamic_cast<Character*>(entity);
        if (!character)
        {
            return false;
        }

        return character->GetGameTag(GameTag::FROZEN) == 1;
    });
}

SelfCondition SelfCondition::HasMinionInHand()
{
    return SelfCondition([=](Entity* entity) -> bool {
        for (auto& card : entity->owner->GetHandZone().GetAll())
        {
            if (dynamic_cast<Minion*>(card) != nullptr)
            {
                return true;
            }
        }

        return false;
    });
}

SelfCondition SelfCondition::IsOverloadCard()
{
    return SelfCondition([=](Entity* entity) -> bool {
        if (entity->GetGameTag(GameTag::OVERLOAD) >= 1)
        {
            return true;
        }

        return false;
    });
}

SelfCondition SelfCondition::MinionsPlayedThisTurn(int num)
{
    return SelfCondition([=](Entity* entity) -> bool {
        return entity->owner->GetNumMinionsPlayedThisTurn() == num;
    });
}

SelfCondition SelfCondition::IsTagValue(GameTag tag, int value,
                                        RelaSign relaSign)
{
    return SelfCondition([=](Entity* entity) -> bool {
        return (relaSign == RelaSign::EQ && entity->GetGameTag(tag) == value) ||
               (relaSign == RelaSign::GEQ &&
                entity->GetGameTag(tag) >= value) ||
               (relaSign == RelaSign::LEQ && entity->GetGameTag(tag) <= value);
    });
}

SelfCondition SelfCondition::IsName(const std::string& name, bool isEqual)
{
    return SelfCondition([=](Entity* entity) -> bool {
        return !((entity->card->name == name) ^ isEqual);
    });
}

SelfCondition SelfCondition::IsStackNum(int value, RelaSign relaSign, int index)
{
    return SelfCondition([=](Entity* entity) -> bool {
        auto& stack = entity->owner->GetGame()->taskStack;
        const auto num = index == 0 ? stack.num : stack.num1;

        return (relaSign == RelaSign::EQ && num == value) ||
               (relaSign == RelaSign::GEQ && num >= value) ||
               (relaSign == RelaSign::LEQ && num <= value);
    });
}

SelfCondition SelfCondition::IsHealth(int value, RelaSign relaSign)
{
    return SelfCondition([=](Entity* entity) -> bool {
        const auto character = dynamic_cast<Character*>(entity);
        if (character == nullptr)
        {
            return false;
        }

        return (relaSign == RelaSign::EQ && character->GetHealth() == value) ||
               (relaSign == RelaSign::GEQ && character->GetHealth() >= value) ||
               (relaSign == RelaSign::LEQ && character->GetHealth() <= value);
    });
}

bool SelfCondition::Evaluate(Entity* entity) const
{
    return m_func(entity);
}
}  // namespace RosettaStone
