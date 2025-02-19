// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on peter1591's hearthstone-ai repository.
// References: https://github.com/peter1591/hearthstone-ai

#include <Rosetta/Views/BoardRefView.hpp>
#include <Rosetta/Views/ReducedBoardView.hpp>

namespace RosettaStone
{
ReducedBoardView::ReducedBoardView(const BoardRefView& view)
    : m_turn(view.GetTurn()), m_playerType(view.GetPlayerType())
{
    m_myHero.Fill(*view.GetHero(), view.IsHeroAttackable(m_playerType));
    m_myHeroPower.Fill(*view.GetHeroPower(m_playerType));
    m_myWeapon.Invalidate();
    if (view.GetWeapon(m_playerType) != nullptr)
    {
        m_myWeapon.Fill(*view.GetWeapon(m_playerType));
    }
    m_myManaCrystal.Fill(view.GetCurrentPlayer());
    for (auto& minion : view.GetMinions(m_playerType))
    {
        ViewTypes::MyMinion myMinion;
        myMinion.Fill(*minion, minion->CanAttack());

        m_myMinions.emplace_back(myMinion);
    }
    for (auto& card : view.GetHandCards())
    {
        ViewTypes::MyHandCard myHandCard;
        myHandCard.Fill(*card);

        m_myHand.emplace_back(myHandCard);
    }
    m_myDeck.Fill(view.GetDeckCardCount(m_playerType));

    const PlayerType opPlayerType =
        (m_playerType == PlayerType::PLAYER1 ? PlayerType::PLAYER2
                                             : PlayerType::PLAYER1);

    m_opHero.Fill(*view.GetOpponentHero());
    m_opHeroPower.Fill(*view.GetHeroPower(opPlayerType));
    m_opWeapon.Invalidate();
    if (view.GetWeapon(opPlayerType) != nullptr)
    {
        m_opWeapon.Fill(*view.GetWeapon(opPlayerType));
    }
    m_opManaCrystal.Fill(*view.GetCurrentPlayer().opponent);
    for (auto& minion : view.GetMinions(opPlayerType))
    {
        ViewTypes::MyMinion opMinion;
        opMinion.Fill(*minion, minion->CanAttack());

        m_opMinions.emplace_back(opMinion);
    }
    for (std::size_t i = 0; i < view.GetOpponentHandCards().size(); ++i)
    {
        ViewTypes::OpHandCard opHandCard;
        m_opHand.emplace_back(opHandCard);
    }
    m_opDeck.Fill(view.GetDeckCardCount(opPlayerType));
}

bool ReducedBoardView::operator==(const ReducedBoardView& rhs) const
{
    return (m_turn == rhs.m_turn) && (m_playerType == rhs.m_playerType) &&
           (m_myHero == rhs.m_myHero) && (m_myHeroPower == rhs.m_myHeroPower) &&
           (m_myWeapon == rhs.m_myWeapon) &&
           (m_myManaCrystal == rhs.m_myManaCrystal) &&
           (m_myMinions == rhs.m_myMinions) && (m_myHand == rhs.m_myHand) &&
           (m_myDeck == rhs.m_myDeck) && (m_opHero == rhs.m_opHero) &&
           (m_opHeroPower == rhs.m_opHeroPower) &&
           (m_opWeapon == rhs.m_opWeapon) &&
           (m_opManaCrystal == rhs.m_opManaCrystal) &&
           (m_opMinions == rhs.m_opMinions) && (m_opHand == rhs.m_opHand) &&
           (m_opDeck == rhs.m_opDeck);
}

bool ReducedBoardView::operator!=(const ReducedBoardView& rhs) const
{
    return !(*this == rhs);
}

int ReducedBoardView::GetTurn() const
{
    return m_turn;
}

PlayerType ReducedBoardView::GetPlayerType() const
{
    return m_playerType;
}

const ViewTypes::MyHero& ReducedBoardView::GetMyHero() const
{
    return m_myHero;
}

const ViewTypes::HeroPower& ReducedBoardView::GetMyHeroPower() const
{
    return m_myHeroPower;
}

const ViewTypes::Weapon& ReducedBoardView::GetMyWeapon() const
{
    return m_myWeapon;
}

const ViewTypes::ManaCrystal& ReducedBoardView::GetMyManaCrystal() const
{
    return m_myManaCrystal;
}

const ViewTypes::MyMinions& ReducedBoardView::GetMyMinions() const
{
    return m_myMinions;
}

const ViewTypes::MyHand& ReducedBoardView::GetMyHand() const
{
    return m_myHand;
}

const ViewTypes::Deck& ReducedBoardView::GetMyDeck() const
{
    return m_myDeck;
}

const ViewTypes::Hero& ReducedBoardView::GetOpHero() const
{
    return m_opHero;
}

const ViewTypes::HeroPower& ReducedBoardView::GetOpHeroPower() const
{
    return m_opHeroPower;
}

const ViewTypes::Weapon& ReducedBoardView::GetOpWeapon() const
{
    return m_opWeapon;
}

const ViewTypes::ManaCrystal& ReducedBoardView::GetOpManaCrystal() const
{
    return m_opManaCrystal;
}

const ViewTypes::Minions& ReducedBoardView::GetOpMinions() const
{
    return m_opMinions;
}

const ViewTypes::OpHand& ReducedBoardView::GetOpHand() const
{
    return m_opHand;
}

const ViewTypes::Deck& ReducedBoardView::GetOpDeck() const
{
    return m_opDeck;
}
}  // namespace RosettaStone