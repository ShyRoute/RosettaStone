// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Rosetta/Commons/Utils.hpp>
#include <Rosetta/Models/HeroPower.hpp>
#include <Rosetta/Models/Player.hpp>
#include <Rosetta/Policies/Policy.hpp>
#include <Rosetta/Tasks/PlayerTasks/AttackTask.hpp>
#include <Rosetta/Tasks/PlayerTasks/EndTurnTask.hpp>
#include <Rosetta/Tasks/PlayerTasks/PlayCardTask.hpp>

namespace RosettaStone
{
Player::Player() : playerID(USER_INVALID)
{
    m_deckZone = new DeckZone(this);
    m_fieldZone = new FieldZone(this);
    m_graveyardZone = new GraveyardZone(this);
    m_handZone = new HandZone(this);
    m_secretZone = new SecretZone(this);
    m_setasideZone = new SetasideZone(this);
}

Player::~Player()
{
    delete m_setasideZone;
    delete m_secretZone;
    delete m_handZone;
    delete m_graveyardZone;
    delete m_fieldZone;
    delete m_deckZone;

    delete m_hero;
}

void Player::RefCopy(const Player& rhs)
{
    nickname = rhs.nickname;
    playerType = rhs.playerType;
    playerID = rhs.playerID;

    playState = rhs.playState;
    mulliganState = rhs.mulliganState;
    choice = rhs.choice;

    policy = rhs.policy;
    opponent = rhs.opponent;

    currentSpellPower = rhs.currentSpellPower;

    m_deckZone = rhs.m_deckZone;
    m_fieldZone = rhs.m_fieldZone;
    m_graveyardZone = rhs.m_graveyardZone;
    m_handZone = rhs.m_handZone;
    m_secretZone = rhs.m_secretZone;
    m_setasideZone = rhs.m_setasideZone;

    m_hero = rhs.m_hero;
    m_game = rhs.m_game;

    m_gameTags = rhs.m_gameTags;
}

Game* Player::GetGame() const
{
    return m_game;
}

void Player::SetGame(Game* game)
{
    m_game = game;
}

FieldZone& Player::GetFieldZone() const
{
    return *m_fieldZone;
}

DeckZone& Player::GetDeckZone() const
{
    return *m_deckZone;
}

GraveyardZone& Player::GetGraveyardZone() const
{
    return *m_graveyardZone;
}

HandZone& Player::GetHandZone() const
{
    return *m_handZone;
}

SecretZone& Player::GetSecretZone() const
{
    return *m_secretZone;
}

SetasideZone& Player::GetSetasideZone() const
{
    return *m_setasideZone;
}

Hero* Player::GetHero() const
{
    return m_hero;
}

int Player::GetGameTag(GameTag tag) const
{
    if (m_gameTags.find(tag) == m_gameTags.end())
    {
        return 0;
    }

    return m_gameTags.at(tag);
}

void Player::SetGameTag(GameTag tag, int value)
{
    m_gameTags.insert_or_assign(tag, value);
}

int Player::GetTotalMana() const
{
    return GetGameTag(GameTag::RESOURCES);
}

void Player::SetTotalMana(int amount)
{
    SetGameTag(GameTag::RESOURCES, amount);
}

int Player::GetUsedMana() const
{
    return GetGameTag(GameTag::RESOURCES_USED);
}

void Player::SetUsedMana(int amount)
{
    SetGameTag(GameTag::RESOURCES_USED, amount);
}

int Player::GetTemporaryMana() const
{
    return GetGameTag(GameTag::TEMP_RESOURCES);
}

void Player::SetTemporaryMana(int amount)
{
    SetGameTag(GameTag::TEMP_RESOURCES, amount);
}

int Player::GetOverloadOwed() const
{
    return GetGameTag(GameTag::OVERLOAD_OWED);
}

void Player::SetOverloadOwed(int amount)
{
    SetGameTag(GameTag::OVERLOAD_OWED, amount);
}

int Player::GetOverloadLocked() const
{
    return GetGameTag(GameTag::OVERLOAD_LOCKED);
}

void Player::SetOverloadLocked(int amount)
{
    SetGameTag(GameTag::OVERLOAD_LOCKED, amount);
}

int Player::GetRemainingMana() const
{
    return GetTotalMana() + GetTemporaryMana() -
           (GetUsedMana() + GetOverloadLocked());
}

bool Player::IsComboActive() const
{
    return GetGameTag(GameTag::COMBO_ACTIVE) == 1;
}

void Player::SetComboActive(bool isActive)
{
    SetGameTag(GameTag::COMBO_ACTIVE, isActive ? 1 : 0);
}

int Player::GetNumMinionsPlayedThisTurn() const
{
    return GetGameTag(GameTag::NUM_MINIONS_PLAYED_THIS_TURN);
}

void Player::SetNumMinionsPlayedThisTurn(int value)
{
    SetGameTag(GameTag::NUM_MINIONS_PLAYED_THIS_TURN, value);
}

ITask* Player::GetNextAction()
{
    ITask* ret;

    do
    {
        // Get next action as TaskID
        // ex) TaskID::END_TURN, TaskID::ATTACK, TaskID::PLAY_CARD
        TaskMeta next = policy->Next(*m_game);
        // Get requirements for proper action
        TaskMeta req = policy->Require(*this, next.GetID());

        ret = GetTaskByAction(next, req);
    } while (ret == nullptr);

    return ret;
}

void Player::AddHeroAndPower(Card* heroCard, Card* powerCard)
{
    m_hero =
        dynamic_cast<Hero*>(Entity::GetFromCard(*this, heroCard));
    m_hero->heroPower = dynamic_cast<HeroPower*>(
        Entity::GetFromCard(*this, powerCard));
}

ITask* Player::GetTaskByAction(TaskMeta& next, TaskMeta& req)
{
    SizedPtr<Entity*> list = req.GetObject<SizedPtr<Entity*>>();

    switch (next.GetID())
    {
        case TaskID::ATTACK:
        {
            if (list.size() >= 2)
            {
                Entity* source = list[0];
                Entity* target = list[1];
                return new PlayerTasks::AttackTask(source, target);
            }

            return nullptr;
        }
        case TaskID::PLAY_CARD:
        {
            // If requirement doesn't satisfy
            if (list.size() < 1)
            {
                return nullptr;
            }

            Entity* source = list[0];
            Entity* target = list.size() >= 2 ? list[1] : nullptr;

            return new PlayerTasks::PlayCardTask(source, target);
        }
        case TaskID::END_TURN:
        {
            return new PlayerTasks::EndTurnTask();
        }
        default:
            return nullptr;
    }
}
}  // namespace RosettaStone
