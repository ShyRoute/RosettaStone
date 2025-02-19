﻿// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef ROSETTASTONE_CARDS_HPP
#define ROSETTASTONE_CARDS_HPP

#include <Rosetta/Cards/Card.hpp>

#include <vector>

namespace RosettaStone
{
//!
//! \brief Search filter structure.
//!
//! This structure stores the filter value for searching the card.
//!
struct SearchFilter
{
    Rarity rarity = Rarity::INVALID;
    CardClass playerClass = CardClass::INVALID;
    CardType cardType = CardType::INVALID;
    Race race = Race::INVALID;
    GameTag gameTag = GameTag::INVALID;

    std::string name;
    int costMin = 0, costMax = 0;
    int attackMin = 0, attackMax = 0;
    int healthMin = 0, healthMax = 0;
};

//!
//! \brief Cards class.
//!
//! This class stores a list of cards and provides several search methods.
//!
class Cards
{
 public:
    //! Deleted copy constructor.
    Cards(const Cards& cards) = delete;

    //! Deleted move constructor.
    Cards(Cards&& cards) = delete;

    //! Deleted copy assignment operator.
    Cards& operator=(const Cards& cards) = delete;

    //! Deleted move assignment operator.
    Cards& operator=(Cards&& cards) = delete;

    //! Returns an instance of Cards class.
    //! \return An instance of Cards class.
    static Cards& GetInstance();

    //! Returns a list of all cards.
    //! \return A list of all cards.
    static const std::vector<Card*>& GetAllCards();

    //! Returns a card that matches \p id.
    //! \param id The ID of the card.
    //! \return A card that matches \p id.
    static Card* FindCardByID(const std::string& id);

    //! Returns a card that matches \p dbfID.
    //! \param dbfID The dbfID of the card.
    //! \return A card that matches \p dbfID.
    static Card* FindCardByDbfID(int dbfID);

    //! Returns a list of cards that matches \p rarity.
    //! \param rarity The rarity of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByRarity(Rarity rarity);

    //! Returns a list of cards that matches \p cardClass.
    //! \param cardClass The class of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByClass(CardClass cardClass);

    //! Returns a list of cards that matches \p cardSet.
    //! \param cardSet The set of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardBySet(CardSet cardSet);

    //! Returns a list of cards that matches \p cardType.
    //! \param cardType The type of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByType(CardType cardType);

    //! Returns a list of cards that matches \p race.
    //! \param race The race of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByRace(Race race);

    //! Returns a card that matches \p name.
    //! \param name The name of the card.
    //! \return A card that matches condition.
    static Card* FindCardByName(const std::string& name);

    //! Returns a list of cards whose cost is between \p minVal and \p maxVal.
    //! \param minVal The minimum cost value of the card.
    //! \param maxVal The maximum cost value of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByCost(int minVal, int maxVal);

    //! Returns a list of cards whose attack is between \p minVal and \p maxVal.
    //! \param minVal The minimum attack value of the card.
    //! \param maxVal The maximum attack value of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByAttack(int minVal, int maxVal);

    //! Returns a list of cards whose health is between \p minVal and \p maxVal.
    //! \param minVal The minimum health value of the card.
    //! \param maxVal The maximum health value of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByHealth(int minVal, int maxVal);

    //! Returns a list of cards whose spell power is between
    //! \p minVal and \p maxVal.
    //! \param minVal The minimum spell power value of the card.
    //! \param maxVal The maximum spell power value of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardBySpellPower(int minVal, int maxVal);

    //! Returns a list of cards that has \p gameTags.
    //! \param gameTags A list of game tag of the card.
    //! \return A list of cards that matches condition.
    static std::vector<Card*> FindCardByGameTag(std::vector<GameTag> gameTags);

    //! Returns a hero card that matches \p cardClass.
    //! \param cardClass The class of the card.
    //! \return A hero card that matches condition.
    static Card* GetHeroCard(CardClass cardClass);

    //! Returns a default hero power card that matches \p cardClass.
    //! \param cardClass The class of the card.
    //! \return A default hero power card that matches condition.
    static Card* GetDefaultHeroPower(CardClass cardClass);

 private:
    //! Constructor: Loads card data.
    Cards();

    //! Destructor: Releases card data.
    ~Cards();

    static std::vector<Card*> m_cards;
};
}  // namespace RosettaStone

#endif  // ROSETTASTONE_CARDS_HPP
