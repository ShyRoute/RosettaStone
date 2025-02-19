// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <Utils/TestUtils.hpp>
#include "gtest/gtest.h"

#include <Rosetta/Actions/ActionParams.hpp>
#include <Rosetta/Actions/Draw.hpp>
#include <Rosetta/Cards/Cards.hpp>
#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Games/GameConfig.hpp>
#include <Rosetta/Games/GameManager.hpp>
#include <Rosetta/Policies/BasicPolicy.hpp>
#include <Rosetta/Tasks/PlayerTasks/AttackTask.hpp>
#include <Rosetta/Tasks/PlayerTasks/EndTurnTask.hpp>
#include <Rosetta/Tasks/PlayerTasks/PlayCardTask.hpp>
#include <Rosetta/Views/Board.hpp>

#include <effolkronium/random.hpp>

using Random = effolkronium::random_static;

using namespace RosettaStone;
using namespace PlayerTasks;

struct MulliganTestPolicy : BasicPolicy
{
    TaskMeta RequireMulligan(Player&) override
    {
        return TaskMeta(TaskMetaTrait(TaskID::MULLIGAN),
                        std::vector<std::size_t>());
    }
};

class TestActionParams : public ActionParams
{
 public:
    TestActionParams() = default;

    TestActionParams(const TestActionParams&) = delete;
    TestActionParams& operator=(const TestActionParams&) = delete;

    void Initialize(const Board& board)
    {
        m_board = &board;
        ActionParams::Initialize(
            m_board->GetCurPlayerStateRefView().GetActionValidGetter());
    }

    std::size_t GetNumber(ActionType actionType, ActionChoices& choices) final
    {
        if (actionType != ActionType::MAIN_ACTION)
        {
            if (choices.Size() == 1)
            {
                return choices.Get(0);
            }
        }

        const std::size_t count = choices.Size();
        const auto randIdx = Random::get<std::size_t>(0, count - 1);
        const int result = static_cast<int>(randIdx);

        return result;
    }

 private:
    const Board* m_board = nullptr;
};

TEST(Game, RefCopyFrom)
{
    GameConfig config1;
    config1.player1Class = CardClass::WARRIOR;
    config1.player2Class = CardClass::ROGUE;
    config1.startPlayer = PlayerType::PLAYER1;
    config1.doFillDecks = true;
    config1.autoRun = true;

    GameConfig config2;
    config2.player1Class = CardClass::MAGE;
    config2.player2Class = CardClass::HUNTER;
    config2.startPlayer = PlayerType::PLAYER2;
    config2.doFillDecks = true;
    config2.autoRun = false;

    Game* game1 = new Game(config1);
    game1->step = Step::BEGIN_DRAW;
    game1->nextStep = Step::MAIN_ACTION;

    Game* game2 = new Game(config2);
    game2->step = Step::FINAL_WRAPUP;
    game2->nextStep = Step::MAIN_COMBAT;

    game1->RefCopyFrom(*game2);
    EXPECT_EQ(game1->step, Step::FINAL_WRAPUP);
    EXPECT_EQ(game1->nextStep, Step::MAIN_COMBAT);

    delete game1;
}

TEST(Game, GetPlayer)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    const Game game(config);

    const Player& player1 = game.GetPlayer1();
    EXPECT_EQ(player1.playerType, PlayerType::PLAYER1);

    const Player& player2 = game.GetPlayer2();
    EXPECT_EQ(player2.playerType, PlayerType::PLAYER2);
}

TEST(Game, GetTurn)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    auto& curPlayer = game.GetCurrentPlayer();
    auto& opPlayer = game.GetOpponentPlayer();

    EXPECT_EQ(game.GetTurn(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(game.GetTurn(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    EXPECT_EQ(game.GetTurn(), 3);
}

TEST(Game, Mulligan)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.skipMulligan = false;
    config.autoRun = false;

    Game game(config);
    game.StartGame();

    auto& curPlayer = game.GetCurrentPlayer();
    auto& opPlayer = game.GetOpponentPlayer();

    MulliganTestPolicy policy;
    curPlayer.policy = &policy;
    opPlayer.policy = &policy;

    game.nextStep = Step::BEGIN_MULLIGAN;
    GameManager::ProcessNextStep(game, game.nextStep);
}

TEST(Game, GameOver_Player1Won)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);
    opPlayer.GetHero()->SetDamage(29);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    game.Process(curPlayer, AttackTask(card1, opPlayer.GetHero()));

    EXPECT_EQ(game.state, State::COMPLETE);
    EXPECT_EQ(curPlayer.playState, PlayState::WON);
    EXPECT_EQ(opPlayer.playState, PlayState::LOST);
}

TEST(Game, GameOver_Player2Won)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER2;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);
    opPlayer.GetHero()->SetDamage(29);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Wolfrider"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    game.Process(curPlayer, AttackTask(card1, opPlayer.GetHero()));

    EXPECT_EQ(game.state, State::COMPLETE);
    EXPECT_EQ(curPlayer.playState, PlayState::WON);
    EXPECT_EQ(opPlayer.playState, PlayState::LOST);
}

TEST(Game, GameOver_Tied)
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    Player& curPlayer = game.GetCurrentPlayer();
    Player& opPlayer = game.GetOpponentPlayer();
    curPlayer.SetTotalMana(10);
    curPlayer.SetUsedMana(0);
    opPlayer.SetTotalMana(10);
    opPlayer.SetUsedMana(0);
    curPlayer.GetHero()->SetDamage(29);
    opPlayer.GetHero()->SetDamage(29);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::GetInstance().FindCardByName("Hellfire"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));

    EXPECT_EQ(game.state, State::COMPLETE);
    EXPECT_EQ(curPlayer.playState, PlayState::TIED);
    EXPECT_EQ(opPlayer.playState, PlayState::TIED);
}

TEST(Game, PerformAction)
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doShuffle = false;
    config.doFillDecks = false;
    config.skipMulligan = true;
    config.autoRun = true;

    std::array<std::string, START_DECK_SIZE> deck = {
        "CS2_106", "CS2_105", "CS1_112", "CS1_112",  // 1
        "CS1_113", "CS1_113", "EX1_154", "EX1_154",  // 2
        "EX1_154", "EX1_154", "CS2_022", "CS2_022",  // 3
        "CS2_023", "CS2_023", "CS2_024", "CS2_024",  // 4
        "CS2_025", "CS2_025", "CS2_026", "CS2_026",  // 5
        "CS2_027", "CS2_027", "CS2_029", "CS2_029",  // 6
        "CS2_032", "CS2_032", "CS2_033", "CS2_033",  // 7
        "CS2_037", "CS2_037"
    };

    for (std::size_t j = 0; j < START_DECK_SIZE; ++j)
    {
        config.player1Deck[j] = *Cards::FindCardByID(deck[j]);
        config.player2Deck[j] = *Cards::FindCardByID(deck[j]);
    }

    Game game(config);
    game.StartGame();
    game.MainReady();

    while (game.state != State::COMPLETE)
    {
        TestActionParams params;
        Board board(game, game.GetCurrentPlayer().playerType);

        params.Initialize(board);
        board.ApplyAction(params);
    }

    EXPECT_EQ(game.state, State::COMPLETE);
    EXPECT_TRUE(game.GetPlayer1().playState == PlayState::WON ||
                game.GetPlayer1().playState == PlayState::LOST);
    EXPECT_TRUE(game.GetPlayer2().playState == PlayState::WON ||
                game.GetPlayer2().playState == PlayState::LOST);
}

TEST(Game, CreateView)
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::ROGUE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.StartGame();
    game.ProcessUntil(Step::MAIN_START);

    const auto player1View = game.CreateView();
    EXPECT_EQ(player1View.GetMyHeroPower().cardID, "CS2_102");

    game.Process(game.GetCurrentPlayer(), EndTurnTask());
    game.ProcessUntil(Step::MAIN_START);

    const auto player2View = game.CreateView();
    EXPECT_EQ(player2View.GetMyHeroPower().cardID, "CS2_083b");
}