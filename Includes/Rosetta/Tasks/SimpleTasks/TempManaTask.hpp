// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_TEMP_MANA_TASK_HPP
#define ROSETTASTONE_TEMP_MANA_TASK_HPP

#include <Rosetta/Tasks/ITask.hpp>

namespace RosettaStone::SimpleTasks
{
//!
//! \brief TempManaTask class.
//!
//! This class represents the task for giving temporary mana crystal.
//!
class TempManaTask : public ITask
{
 public:
    //! Constructs task with given \p amount.
    //! \param amount A value indicating how much to give mana crystal.
    explicit TempManaTask(int amount);

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

    int m_amount;
};
}  // namespace RosettaStone::SimpleTasks

#endif  // ROSETTASTONE_TEMP_MANA_TASK_HPP
