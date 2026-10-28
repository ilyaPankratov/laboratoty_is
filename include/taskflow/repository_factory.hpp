#pragma once
#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/sqlite_task_repository.hpp"
#include <memory>
#include <stdexcept>
#include <string>

namespace taskflow {

struct Desk {
    std::string slot{"memory"}; // memory | sqlite
    std::string db_name{"pankratov.db"};
};

inline TaskRepositoryPtr make_repository(const Desk& desk) {
    if (desk.slot == "memory") {
        return std::make_shared<InMemoryTaskRepository>();
    }
    if (desk.slot == "sqlite") {
        return std::make_shared<SQLiteTaskRepository>(desk.db_name);
    }
    throw std::invalid_argument("bad slot: " + desk.slot);
}

} // namespace taskflow
