#pragma once
#include "taskflow/priority_strategy.hpp"
#include "taskflow/repository.hpp"
#include <functional>
#include <string>
#include <vector>

namespace taskflow {

using WatchFn = std::function<void(const std::string& line)>;

class TaskService {
public:
    TaskService(TaskRepositoryPtr store, PriorityStrategyPtr ranking);

    Task register_task(std::string title, std::string assignee, int priority,
                       std::string description = "", int effort = 2);
    bool hand_off(std::int64_t id, std::string assignee);
    bool advance(std::int64_t id, TaskStatus next);
    std::vector<Task> board() const;
    std::optional<Task> lookup(std::int64_t id) const;

    void watch(WatchFn fn);

private:
    void check_fields(const std::string& title, const std::string& assignee,
                      int priority, const std::string& description, int effort) const;
    bool allowed(TaskStatus from, TaskStatus to) const;

    TaskRepositoryPtr store_;
    PriorityStrategyPtr ranking_;
    WatchFn watch_;
};

} // namespace taskflow
