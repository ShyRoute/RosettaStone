// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on peter1591's hearthstone-ai repository.
// References: https://github.com/peter1591/hearthstone-ai

#include <Rosetta/Views/BoardRefView.hpp>

namespace RosettaStone
{
BoardRefView::BoardRefView(const Game& game, PlayerType playerType)
    : m_game(game), m_playerType(playerType)
{
    // Do nothing
}

int BoardRefView::GetTurn() const
{
    return m_game.GetTurn();
}

PlayerType BoardRefView::GetPlayerType() const
{
    return m_playerType;
}

Player& BoardRefView::GetCurrentPlayer() const
{
    return m_game.GetCurrentPlayer();
}

int BoardRefView::GetFatigueDamage(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetHero()->fatigue;
    }
    else
    {
        return m_game.GetPlayer2().GetHero()->fatigue;
    }
}

int BoardRefView::GetTotalMana(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetTotalMana();
    }
    else
    {
        return m_game.GetPlayer2().GetTotalMana();
    }
}

int BoardRefView::GetUsedMana(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetUsedMana();
    }
    else
    {
        return m_game.GetPlayer2().GetUsedMana();
    }
}

int BoardRefView::GetTemporaryMana(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetTemporaryMana();
    }
    else
    {
        return m_game.GetPlayer2().GetTemporaryMana();
    }
}

int BoardRefView::GetOverloadOwed(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetOverloadOwed();
    }
    else
    {
        return m_game.GetPlayer2().GetOverloadOwed();
    }
}

int BoardRefView::GetOverloadLocked(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetOverloadLocked();
    }
    else
    {
        return m_game.GetPlayer2().GetOverloadLocked();
    }
}

int BoardRefView::GetRemainingMana(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetRemainingMana();
    }
    else
    {
        return m_game.GetPlayer2().GetRemainingMana();
    }
}

Hero* BoardRefView::GetHero() const
{
    if (m_playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetHero();
    }
    else
    {
        return m_game.GetPlayer2().GetHero();
    }
}
Hero* BoardRefView::GetOpponentHero() const
{
    if (m_playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer2().GetHero();
    }
    else
    {
        return m_game.GetPlayer1().GetHero();
    }
}

HeroPower* BoardRefView::GetHeroPower(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetHero()->heroPower;
    }
    else
    {
        return m_game.GetPlayer2().GetHero()->heroPower;
    }
}

Weapon* BoardRefView::GetWeapon(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetHero()->weapon;
    }
    else
    {
        return m_game.GetPlayer2().GetHero()->weapon;
    }
}

std::vector<Entity*> BoardRefView::GetHandCards() const
{
    if (m_playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetHandZone().GetAll();
    }
    else
    {
        return m_game.GetPlayer2().GetHandZone().GetAll();
    }
}

std::vector<std::pair<Entity*, bool>> BoardRefView::GetOpponentHandCards() const
{
    std::vector<Entity*> entities;

    if (m_playerType == PlayerType::PLAYER1)
    {
        entities = m_game.GetPlayer2().GetHandZone().GetAll();
    }
    else
    {
        entities = m_game.GetPlayer1().GetHandZone().GetAll();
    }

    std::vector<std::pair<Entity*, bool>> result;

    for (auto& entity : entities)
    {
        if (entity->card->id == "GAME_005")
        {
            // The Coin. This also reveals to opponent.
            result.emplace_back(std::make_pair(entity, true));
        }
        else
        {
            result.emplace_back(std::make_pair(entity, false));
        }
    }

    return result;
}

int BoardRefView::GetOpponentHandCardCount() const
{
    if (m_playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer2().GetHandZone().GetCount();
    }
    else
    {
        return m_game.GetPlayer1().GetHandZone().GetCount();
    }
}

std::vector<Minion*> BoardRefView::GetMinions(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetFieldZone().GetAll();
    }
    else
    {
        return m_game.GetPlayer2().GetFieldZone().GetAll();
    }
}

int BoardRefView::GetDeckCardCount(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetDeckZone().GetCount();
    }
    else
    {
        return m_game.GetPlayer2().GetDeckZone().GetCount();
    }
}

bool BoardRefView::IsHeroAttackable(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetHero()->CanAttack();
    }
    else
    {
        return m_game.GetPlayer2().GetHero()->CanAttack();
    }
}

bool BoardRefView::IsMinionAttackable(PlayerType playerType, int idx) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return m_game.GetPlayer1().GetFieldZone()[idx]->CanAttack();
    }
    else
    {
        return m_game.GetPlayer2().GetFieldZone()[idx]->CanAttack();
    }
}

CurrentPlayerBoardRefView::CurrentPlayerBoardRefView(const Game& game)
    : m_game(game)
{
    // Do nothing
}

Player& CurrentPlayerBoardRefView::GetCurrentPlayer() const
{
    return m_game.GetCurrentPlayer();
}

ActionValidGetter CurrentPlayerBoardRefView::GetActionValidGetter() const
{
    return ActionValidGetter(m_game);
}
}  // namespace RosettaStone