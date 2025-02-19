// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef ROSETTASTONE_ITASK_HPP
#define ROSETTASTONE_ITASK_HPP

#include <Rosetta/Models/Player.hpp>
#include <Rosetta/Tasks/TaskMeta.hpp>
#include <Rosetta/Tasks/TaskStatus.hpp>

namespace RosettaStone
{
//!
//! \brief ITask class.
//!
//! This class is interface of various task classes.
//! All classes that inherit from it must implement GetTaskID and Impl methods.
//!
class ITask
{
 public:
    template <typename T>
    static inline constexpr bool isTask =
        std::is_convertible_v<std::decay_t<T>, ITask>;

    //! Default constructor.
    ITask() = default;

    //! Constructs task with given \p entityType.
    //! \param entityType The entity type of target.
    ITask(EntityType entityType);

    //! Constructs task with given \p source and \p target.
    //! \param source A source entity to apply task.
    //! \param target A target entity to apply task.
    ITask(Entity* source, Entity* target);

    //! Constructs task with given \p entityType, \p source and \p target.
    //! \param entityType The entity type of target.
    //! \param source A source entity to apply task.
    //! \param target A target entity to apply task.
    ITask(EntityType entityType, Entity* source, Entity* target);

    //! Default destructor.
    virtual ~ITask() = default;

    //! Default copy constructor.
    ITask(const ITask& task) = default;

    //! Default move constructor.
    ITask(ITask&& task) = default;

    //! Default copy assignment operator.
    ITask& operator=(const ITask& task) = default;

    //! Default move assignment operator.
    ITask& operator=(ITask&& task) = default;

    //! Returns entity type.
    //! \return Entity type.
    EntityType GetEntityType() const;

    //! Sets the player.
    //! \param player The player to run task.
    void SetPlayer(Player* player);

    //! Sets the source.
    //! \param source The source.
    void SetSource(Entity* source);

    //! Sets the target.
    //! \param target The target.
    void SetTarget(Entity* target);

    //! Calls Impl method and returns meta data.
    //! \return The result of task processing.
    TaskStatus Run();

    //! Returns task ID (pure virtual).
    //! \return Task ID.
    virtual TaskID GetTaskID() const = 0;

    //! Returns the cloned task.
    //! \return The cloned task.
    //! \note This will be used for solving multi-thread issue.
    //! Not to access same elements at same time.
    ITask* Clone();

    //! Checks it is freeable task.
    //! \return The flag to indicate that it is freeable task.
    //! \note This will be used for solving multi-thread and memory leak issue.
    //! \note You have to free it if it is cloned, otherwise cause leaks memory.
    bool IsFreeable() const;

    //! Enables that it is freeable task.
    void EnableFreeable();

 protected:
    EntityType m_entityType = EntityType::EMPTY;
    Player* m_player = nullptr;
    Entity* m_source = nullptr;
    Entity* m_target = nullptr;

    bool m_isFreeable = false;

 private:
    //! Processes task logic internally and returns meta data.
    //! \param player The player to run task.
    //! \return The result of task processing.
    virtual TaskStatus Impl(Player& player) = 0;

    //! Internal method of Clone().
    //! \return The cloned task.
    virtual ITask* CloneImpl() = 0;
};

namespace Task
{
//! Calls Impl method and returns meta data.
//! \param task The task to run.
//! \return The result of task processing.
inline TaskStatus Run(ITask* task)
{
    return task->Run();
}

//! Calls Impl method and returns meta data.
//! \param task The task to run.
//! \return The result of task processing.
inline TaskStatus Run(ITask&& task)
{
    return task.Run();
}

//! Runs multiple Tasks.
//! \param task Packed multiple tasks.
template <typename... TaskType>
std::vector<TaskStatus> RunMulti(TaskType&&... task)
{
    std::vector<TaskStatus> metas;
    metas.reserve(sizeof...(task));

    (..., metas.push_back(task.Run()));
    return metas;
}
}  // namespace Task
}  // namespace RosettaStone

#endif  // ROSETTASTONE_ITASK_HPP
