// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on peter1591's hearthstone-ai repository.
// References: https://github.com/peter1591/hearthstone-ai

#include <Agents/MCTSRunner.hpp>

namespace RosettaTorch::Agents
{
MCTSRunner::MCTSRunner(const MCTSConfig& config) : m_config(config)
{
    // Do nothing
}

MCTSRunner::~MCTSRunner()
{
    WaitUntilStopped();
}

void MCTSRunner::Run(const GameConfig& gameConfig)
{
    m_stopFlag = false;

    for (int i = 0; i < m_config.threads; ++i)
    {
        m_threads.emplace_back([this, gameConfig]() {
            MCTS::MOMCTS mcts(m_p1Tree, m_p2Tree, m_statistics);

            while (!m_stopFlag.load())
            {
                Game game(gameConfig);
                mcts.Iterate(game);

                m_statistics.IterateSucceeded();
            }
        });
    }
}

const MCTS::Statistics<>& MCTSRunner::GetStatistics() const
{
    return m_statistics;
}

const MCTS::TreeNode* MCTSRunner::GetRootNode(PlayerType playerType) const
{
    if (playerType == PlayerType::PLAYER1)
    {
        return &m_p1Tree;
    }
    else
    {
        return &m_p2Tree;
    }
}

void MCTSRunner::NotifyStop()
{
    m_stopFlag = true;
}

void MCTSRunner::WaitUntilStopped()
{
    NotifyStop();

    for (auto& thread : m_threads)
    {
        thread.join();
    }

    m_threads.clear();
}
}  // namespace RosettaTorch::Agents