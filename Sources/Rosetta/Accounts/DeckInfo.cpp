// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Rosetta/Accounts/DeckInfo.hpp>
#include <Rosetta/Cards/Cards.hpp>

#include <algorithm>
#include <iostream>

namespace RosettaStone
{
DeckInfo::DeckInfo() : m_name("Empty")
{
    // Do nothing
}

DeckInfo::DeckInfo(std::string name, const CardClass deckClass)
    : m_name(std::move(name)), m_class(deckClass)
{
    // Do nothing
}

std::string DeckInfo::GetName() const
{
    return m_name;
}

CardClass DeckInfo::GetClass() const
{
    return m_class;
}

std::size_t DeckInfo::GetNumOfCards() const
{
    return m_numOfCards;
}

std::size_t DeckInfo::GetUniqueNumOfCards() const
{
    return m_cards.size();
}

std::size_t DeckInfo::GetNumCardInDeck(std::string cardID)
{
    const auto cardIter = std::find_if(
        m_cards.begin(), m_cards.end(),
        [&cardID](const std::pair<std::string, std::size_t>& elem) {
            return elem.first == cardID;
        });

    // A card is in deck
    if (cardIter != m_cards.end())
    {
        return (*cardIter).second;
    }

    return 0;
}

std::vector<Card*> DeckInfo::GetPrimitiveDeck() const
{
    std::vector<Card*> deck;

    for (const auto& [id, num] : m_cards)
    {
        Card* card = Cards::GetInstance().FindCardByID(id);

        for (std::size_t i = 0; i < num; ++i)
        {
            deck.push_back(card);
        }
    }

    return deck;
}

std::pair<std::string, std::size_t> DeckInfo::GetCard(std::size_t idx) const
{
    return m_cards.at(idx);
}

void DeckInfo::ShowCardList() const
{
    int idx = 1;

    for (auto& cardInfo : m_cards)
    {
        Card* card = Cards::GetInstance().FindCardByID(cardInfo.first);
        if (card->id.empty())
        {
            continue;
        }

        std::cout << idx << ". ";
        card->ShowBriefInfo();
        std::cout << "(" << cardInfo.second << " card(s))\n";

        idx++;
    }
}

bool DeckInfo::AddCard(std::string cardID, std::size_t numCardToAdd)
{
    Card* card = Cards::GetInstance().FindCardByID(cardID);

    const CardClass cardClass = card->GetCardClass();
    if ((cardClass != GetClass() && cardClass != CardClass::NEUTRAL) ||
        card->GetMaxAllowedInDeck() < numCardToAdd)
    {
        return false;
    }

    const auto cardIter = std::find_if(
        m_cards.begin(), m_cards.end(),
        [&cardID](const std::pair<std::string, std::size_t>& elem) {
            return elem.first == cardID;
        });

    // A card is in deck
    if (cardIter != m_cards.end())
    {
        if (card->GetMaxAllowedInDeck() < (*cardIter).second + numCardToAdd)
            return false;

        (*cardIter).second += numCardToAdd;
    }
    else
    {
        m_cards.emplace_back(std::make_pair(cardID, numCardToAdd));
    }

    m_numOfCards += numCardToAdd;

    return true;
}

bool DeckInfo::DeleteCard(std::string cardID, std::size_t numCardToDelete)
{
    const auto cardIter = std::find_if(
        m_cards.begin(), m_cards.end(),
        [&cardID](const std::pair<std::string, std::size_t>& elem) {
            return elem.first == cardID;
        });

    // A card is in deck
    if (cardIter != m_cards.end())
    {
        // Remove a given number of cards from deck
        (*cardIter).second = std::min((*cardIter).second - numCardToDelete,
                                      static_cast<std::size_t>(0u));
        m_numOfCards = std::min(m_numOfCards - numCardToDelete,
                                static_cast<std::size_t>(0u));

        // If the number of cards is less than 0, remove card from deck
        if ((*cardIter).second <= 0)
        {
            m_cards.erase(cardIter);
        }

        return true;
    }

    return false;
}
}  // namespace RosettaStone
