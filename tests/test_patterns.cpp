#include "simple_test.hpp"
#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/priority_strategy.hpp"
#include "taskflow/repository_factory.hpp"
#include "taskflow/slash_parser.hpp"
#include "taskflow/sqlite_task_repository.hpp"
#include "taskflow/task_service.hpp"
#include "taskflow/work_log.hpp"
#include <memory>
#include <string>

using namespace taskflow;

TEST_CASE("work2_factory_memory") {
    Desk desk; desk.slot = "memory";
    auto repo = make_repository(desk);
    Task d; d.title = "a"; d.assignee = "b"; d.priority = 3; d.effort = 2;
    CHECK_TRUE(repo->find_by_id(repo->create(d).id).has_value());
}

TEST_CASE("work2_closed_last") {
    auto repo = std::make_shared<InMemoryTaskRepository>();
    TaskService d(repo, std::make_shared<ClosedLastPriorityStrategy>());
    auto a = d.register_task("A", "u", 1);
    auto b = d.register_task("B", "u", 5);
    d.advance(a.id, TaskStatus::InProgress);
    d.advance(a.id, TaskStatus::Done);
    CHECK_EQ(d.board().front().id, b.id);
}

TEST_CASE("work2_factory_sqlite") {
    Desk desk; desk.slot = "sqlite"; desk.db_name = "pnk.tmp.db";
    auto repo = make_repository(desk);
    auto* p = dynamic_cast<SQLiteTaskRepository*>(repo.get());
    CHECK_TRUE(p != nullptr);
    CHECK_EQ(p->file(), std::string("pnk.tmp.db"));
}

TEST_CASE("work2_factory_bad") {
    Desk desk; desk.slot = "files";
    CHECK_THROWS(make_repository(desk));
}

TEST_CASE("work2_effort_first") {
    auto repo = std::make_shared<InMemoryTaskRepository>();
    TaskService d(repo, std::make_shared<EffortFirstStrategy>());
    auto slow = d.register_task("slow", "u", 1, "", 6);
    auto fast = d.register_task("fast", "u", 5, "", 1);
    CHECK_EQ(d.board().front().id, fast.id);
    CHECK_EQ(d.board().back().id, slow.id);
}

TEST_CASE("work2_work_log") {
    TaskService d(std::make_shared<InMemoryTaskRepository>(),
                  std::make_shared<DefaultPriorityStrategy>());
    WorkLog log;
    d.watch([&](const std::string& line) { log.note(line); });
    auto t = d.register_task("X", "P", 2);
    CHECK_TRUE(log.size() == 1);
    CHECK_EQ(log.lines().front(), std::string("register #") + std::to_string(t.id));
}

TEST_CASE("work2_slash_ok") {
    auto t = parse_slash("Notes/Ann/2/4");
    CHECK_EQ(t.title, std::string("Notes"));
    CHECK_EQ(t.assignee, std::string("Ann"));
    CHECK_EQ(t.priority, 2);
    CHECK_EQ(t.effort, 4);
}

TEST_CASE("work2_slash_bad") { CHECK_THROWS(parse_slash("only/two")); }

TEST_CASE("work2_sqlite_stub") {
    SQLiteTaskRepository r("z.db");
    CHECK_THROWS(r.list());
}
