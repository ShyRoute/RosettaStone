// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_RANDOM_ENTOURAGE_TASK_HPP
#define ROSETTASTONE_RANDOM_ENTOURAGE_TASK_HPP

#include <Rosetta/Tasks/ITask.hpp>

namespace RosettaStone::SimpleTasks
{
//!
//! \brief RandomEntourageTask class.
//!
//! This class represents the task for picking entourage at random.
//!
class RandomEntourageTask : public ITask
{
 public:
    //! Constructs task with given \p count and \p isOpponent.
    //! \param count A value indicating the number of picking.
    //! \param isOpponent A flag to owner indicating opponent player.
    RandomEntourageTask(int count = 1, bool isOpponent = false);

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

    int m_count = 1;
    bool m_isOpponent = false;
};
}  // namespace RosettaStone::SimpleTasks

#endif  // ROSETTASTONE_RANDOM_ENTOURAGE_TASK_HPP
