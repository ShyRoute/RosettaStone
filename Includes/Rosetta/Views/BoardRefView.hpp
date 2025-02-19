// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on peter1591's hearthstone-ai repository.
// References: https://github.com/peter1591/hearthstone-ai

#ifndef ROSETTASTONE_BOARD_REF_VIEW_HPP
#define ROSETTASTONE_BOARD_REF_VIEW_HPP

#include <Rosetta/Actions/ActionValidGetter.hpp>
#include <Rosetta/Games/Game.hpp>

namespace RosettaStone
{
//!
//! \brief BoardRefView class.
//!
//! This class considers when a particular player looks at a game
//! from his or her point of view.
//!
class BoardRefView
{
 public:
    //! Constructs board with given \p game and \p playerType.
    //! \param game The game context.
    //! \param playerType The player type of the board.
    BoardRefView(const Game& game, PlayerType playerType);

    //! Returns the current turn of the game.
    //! \return The current turn of the game.
    int GetTurn() const;

    //! Returns the player type of the view.
    //! \return the player type of the view.
    PlayerType GetPlayerType() const;

    //! Returns the current player of the game.
    //! \return The current player of the game.
    Player& GetCurrentPlayer() const;

    //! Returns the fatigue damage of the player in the current status.
    //! \param playerType The player type to separate players.
    //! \return The fatigue damage of the player in the current status.
    int GetFatigueDamage(PlayerType playerType) const;

    //! Returns the total mana of the player during this turn.
    //! \param playerType The player type to separate players.
    //! \return The total mana of the player during this turn.
    int GetTotalMana(PlayerType playerType) const;

    //! Returns the used mana of the player during this turn.
    //! \param playerType The player type to separate players.
    //! \return The used mana of the player during this turn.
    int GetUsedMana(PlayerType playerType) const;

    //! Returns the temporary mana of the player during this turn.
    //! \param playerType The player type to separate players.
    //! \return The temporary mana of the player during this turn.
    int GetTemporaryMana(PlayerType playerType) const;

    //! Returns the mana that is overload owed of the player during this turn.
    //! \param playerType The player type to separate players.
    //! \return The mana that is overload owed of the player during this turn.
    int GetOverloadOwed(PlayerType playerType) const;

    //! Returns the mana that is overload locked of the player during this turn.
    //! \param playerType The player type to separate players.
    //! \return The mana that is overload locked of the player during this turn.
    int GetOverloadLocked(PlayerType playerType) const;

    //! Returns the remaining mana of the player during this turn.
    //! \param playerType The player type to separate players.
    //! \return The remaining mana of the player during this turn.
    int GetRemainingMana(PlayerType playerType) const;

    //! Returns the hero of the current player.
    //! \return The hero of the current player.
    Hero* GetHero() const;

    //! Returns the hero of the opponent player.
    //! \return The hero of the opponent player.
    Hero* GetOpponentHero() const;

    //! Returns the hero power of the player.
    //! \param playerType The player type to separate players.
    //! \return The hero power of the player.
    HeroPower* GetHeroPower(PlayerType playerType) const;

    //! Returns the weapon of the player.
    //! \param playerType The player type to separate players.
    //! \return The weapon of the player.
    Weapon* GetWeapon(PlayerType playerType) const;

    //! Returns a list of cards in the hand of the current player.
    //! \return A list of cards in the hand of the current player.
    std::vector<Entity*> GetHandCards() const;

    //! Returns a list of cards in the hand of the opponent player.
    //! \return A list of cards in the hand of the opponent player.
    std::vector<std::pair<Entity*, bool>> GetOpponentHandCards() const;

    //! Returns the number of cards in the hand of the opponent player.
    //! \return The number of cards in the hand of the opponent player.
    int GetOpponentHandCardCount() const;

    //! Returns a list of minions in the field of the player.
    //! \param playerType The player type to separate players.
    //! \return A list of minions in the field of the player.
    std::vector<Minion*> GetMinions(PlayerType playerType) const;

    //! Returns the number of cards in the deck of the player.
    //! \param playerType The player type to separate players.
    //! \return The number of cards in the deck of the player.
    int GetDeckCardCount(PlayerType playerType) const;

    //! Returns the flag indicates that whether the hero can attack.
    //! \param playerType The player type to separate players.
    //! \return The flag indicates that whether the hero can attack.
    bool IsHeroAttackable(PlayerType playerType) const;

    //! Returns the flag indicates that whether the minion can attack.
    //! \param playerType The player type to separate players.
    //! \param idx The index of minion in the field.
    //! \return The flag indicates that whether the minion can attack.
    bool IsMinionAttackable(PlayerType playerType, int idx) const;

 private:
    const Game& m_game;
    PlayerType m_playerType;
};

//!
//! \brief CurrentPlayerBoardRefView class.
//!
//! This class is BoardRefView-like class for the current player.
//!
class CurrentPlayerBoardRefView
{
 public:
    //! Constructs current player board ref view with given \p game.
    //! \param game The game context.
    explicit CurrentPlayerBoardRefView(const Game& game);

    //! Returns the current player of the game.
    //! \return The current player of the game.
    Player& GetCurrentPlayer() const;

    //! Returns the action valid getter.
    //! \return The action valid getter.
    ActionValidGetter GetActionValidGetter() const;

 private:
    const Game& m_game;
};
}  // namespace RosettaStone

#endif  // ROSETTASTONE_BOARD_REF_VIEW_HPP
