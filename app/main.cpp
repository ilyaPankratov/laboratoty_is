#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/priority_strategy.hpp"
#include "taskflow/slash_parser.hpp"
#include "taskflow/task_service.hpp"
#include "taskflow/work_log.hpp"
#include <iostream>
#include <memory>

int main() {
    using namespace taskflow;

    auto store = std::make_shared<InMemoryTaskRepository>();
    auto rank = std::make_shared<EffortFirstStrategy>();
    TaskService desk(store, rank);

    WorkLog log;
    desk.watch([&](const std::string& line) { log.note(line); });

    desk.register_task("Собрать конспект", "Pankratov", 2, "черновик", 4);
    auto item = desk.register_task("Прогнать тесты", "Pankratov", 1, "ctest", 1);
    desk.advance(item.id, TaskStatus::InProgress);

    auto imported = parse_slash("Отчет/QA/3/6");
    imported = store->create(imported);

    std::cout << "Pankratov desk\n";
    for (const auto& t : desk.board()) {
        std::cout << "#" << t.id << " " << t.title
                  << " ~" << t.effort << "h @" << t.assignee
                  << " " << to_string(t.status)
                  << " p=" << t.priority << '\n';
    }
    std::cout << "log lines: " << log.size() << '\n';
    for (const auto& line : log.lines()) {
        std::cout << "  " << line << '\n';
    }
    std::cout << "slash id=" << imported.id << '\n';
    return 0;
}
