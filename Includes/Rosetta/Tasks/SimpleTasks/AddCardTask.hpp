// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_ADD_CARD_TASK_HPP
#define ROSETTASTONE_ADD_CARD_TASK_HPP

#include <Rosetta/Tasks/ITask.hpp>

namespace RosettaStone::SimpleTasks
{
//!
//! \brief AddCardTask class.
//!
//! This class represents the task for adding card(s).
//!
class AddCardTask : public ITask
{
 public:
    //! Constructs task with given \p entityType, \p cardID and \p amount.
    //! \param entityType The entity type of target to add card(s).
    //! \param cardID The ID of card to add.
    //! \param amount The number of card to add.
    explicit AddCardTask(EntityType entityType, std::string cardID,
                         int amount = 1);

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

    std::string m_cardID;
    int m_amount = 1;
};
}  // namespace RosettaStone::SimpleTasks

#endif  // ROSETTASTONE_ADD_CARD_TASK_HPP
