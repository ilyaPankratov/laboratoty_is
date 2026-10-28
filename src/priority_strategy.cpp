#include "taskflow/priority_strategy.hpp"

namespace taskflow {

int DefaultPriorityStrategy::score(const Task& task) const {
    return 10 - task.priority;
}

int ClosedLastPriorityStrategy::score(const Task& task) const {
    return finished(task.status) ? 1000 : 10 - task.priority;
}

int EffortFirstStrategy::score(const Task& task) const {
    if (finished(task.status)) return 8000;
    return task.effort * 10 + task.priority;
}

} // namespace taskflow
