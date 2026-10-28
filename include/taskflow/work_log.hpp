#pragma once
#include <string>
#include <vector>

namespace taskflow {

// короткие строки, без самой задачи
class WorkLog {
public:
    void note(const std::string& line) { lines_.push_back(line); }
    const std::vector<std::string>& lines() const { return lines_; }
    std::size_t size() const { return lines_.size(); }

private:
    std::vector<std::string> lines_;
};

} // namespace taskflow
