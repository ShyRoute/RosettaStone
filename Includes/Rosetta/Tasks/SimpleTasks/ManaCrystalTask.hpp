// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_MANA_CRYSTAL_TASK_HPP
#define ROSETTASTONE_MANA_CRYSTAL_TASK_HPP

#include <Rosetta/Tasks/ITask.hpp>

namespace RosettaStone::SimpleTasks
{
//!
//! \brief ManaCrystalTask class.
//!
//! This class represents the task for changing mana crystal of player.
//!
class ManaCrystalTask : public ITask
{
 public:
    //! Constructs task with given \p amount, \p fill and \p isOpponent.
    //! \param amount A value indicating how much to change mana crystal.
    //! \param fill true if mana crystal is filled, and false otherwise.
    //! \param isOpponent true if opponent player is given mana crystal, and
    //! false otherwise.
    explicit ManaCrystalTask(int amount, bool fill, bool isOpponent = false);

    //! Returns task ID.
    //! \return Task ID.
    TaskID GetTaskID() const override;

 private:
    //! Processes task logic internally and returns meta data.
    //! \param player The player to run task.
    //! \return The result of task processing.
    TaskStatus Impl(Player& player) override;

    //! Internal method of Clone().
    //! \return The cloned task.
    ITask* CloneImpl() override;

    int m_amount = 0;
    bool m_fill = false;
    bool m_isOpponent = false;
};
}  // namespace RosettaStone::SimpleTasks

#endif  // ROSETTASTONE_MANA_CRYSTAL_TASK_HPP
