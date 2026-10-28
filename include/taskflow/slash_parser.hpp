#pragma once
#include "taskflow/task.hpp"
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace taskflow {

// формат: title/assignee/priority/effort
// пример: Notes/Pankratov/2/4
inline Task parse_slash(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, '/')) parts.push_back(item);
    if (parts.size() < 4) throw std::invalid_argument("need title/assignee/priority/effort");
    Task t;
    t.title = parts[0];
    t.assignee = parts[1];
    t.priority = std::stoi(parts[2]);
    t.effort = std::stoi(parts[3]);
    t.status = TaskStatus::New;
    return t;
}

} // namespace taskflow
