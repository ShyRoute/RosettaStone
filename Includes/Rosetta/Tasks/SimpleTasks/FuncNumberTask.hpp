// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_FUNC_NUMBER_TASK_HPP
#define ROSETTASTONE_FUNC_NUMBER_TASK_HPP

#include <Rosetta/Tasks/ITask.hpp>

#include <functional>

namespace RosettaStone::SimpleTasks
{
//!
//! \brief FuncNumberTask class.
//!
//! This class represents the task for executing specific function.
//!
class FuncNumberTask : public ITask
{
 public:
    //! Constructs task with given \p func.
    //! \param func The function to execute.
    explicit FuncNumberTask(std::function<void(Entity*)> func);

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

    std::function<void(Entity*)> m_func;
};
}  // namespace RosettaStone::SimpleTasks

#endif  // ROSETTASTONE_FUNC_NUMBER_TASK_HPP
