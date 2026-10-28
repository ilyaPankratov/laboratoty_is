#include "simple_test.hpp"
#include "taskflow/in_memory_task_repository.hpp"
#include "taskflow/priority_strategy.hpp"
#include "taskflow/task_service.hpp"
#include <memory>
#include <string>

using namespace taskflow;

static TaskService desk() {
    return TaskService(std::make_shared<InMemoryTaskRepository>(),
                       std::make_shared<DefaultPriorityStrategy>());
}

TEST_CASE("work1_register_ok") {
    auto d = desk();
    auto t = d.register_task("Doc", "Pankratov", 2, "text", 3);
    CHECK_TRUE(t.id > 0);
    CHECK_EQ(t.effort, 3);
}

TEST_CASE("work1_bad_priority") { CHECK_THROWS(desk().register_task("t", "p", 9)); }

TEST_CASE("work1_flow") {
    auto d = desk();
    auto t = d.register_task("W", "P", 1);
    CHECK_TRUE(d.advance(t.id, TaskStatus::InProgress));
    CHECK_TRUE(d.advance(t.id, TaskStatus::Done));
    CHECK_TRUE(!d.advance(t.id, TaskStatus::InProgress));
}

TEST_CASE("work1_hand_off_ok") {
    auto d = desk();
    auto t = d.register_task("B", "A", 2);
    CHECK_TRUE(d.hand_off(t.id, "B"));
    CHECK_EQ(d.lookup(t.id)->assignee, std::string("B"));
}

TEST_CASE("work1_hand_off_done_ok") {
    auto d = desk();
    auto t = d.register_task("B", "A", 2);
    d.advance(t.id, TaskStatus::InProgress);
    d.advance(t.id, TaskStatus::Done);
    CHECK_TRUE(d.hand_off(t.id, "C"));
}

TEST_CASE("work1_hand_off_cancelled_no") {
    auto d = desk();
    auto t = d.register_task("B", "A", 2);
    d.advance(t.id, TaskStatus::Cancelled);
    CHECK_TRUE(!d.hand_off(t.id, "C"));
}

TEST_CASE("work1_text_too_long") {
    CHECK_THROWS(desk().register_task("t", "a", 2, std::string(801, 'z')));
}

TEST_CASE("work1_text_800_ok") {
    auto t = desk().register_task("t", "a", 2, std::string(800, 'z'));
    CHECK_EQ(t.description.size(), static_cast<std::size_t>(800));
}

TEST_CASE("work1_effort_bad") {
    CHECK_THROWS(desk().register_task("t", "a", 2, "", 0));
    CHECK_THROWS(desk().register_task("t", "a", 2, "", 9));
}

TEST_CASE("work3_empty_title") { CHECK_THROWS(desk().register_task("", "a", 2)); }
TEST_CASE("work3_empty_assignee") { CHECK_THROWS(desk().register_task("t", "", 2)); }
TEST_CASE("work3_prio_low") { CHECK_THROWS(desk().register_task("t", "a", 0)); }
TEST_CASE("work3_prio_high") { CHECK_THROWS(desk().register_task("t", "a", 6)); }

TEST_CASE("work3_cancel") {
    auto d = desk();
    auto t = d.register_task("t", "a", 3);
    CHECK_TRUE(d.advance(t.id, TaskStatus::Cancelled));
}

TEST_CASE("work3_hand_off_empty") {
    auto d = desk();
    auto t = d.register_task("t", "a", 3);
    CHECK_THROWS(d.hand_off(t.id, ""));
}

TEST_CASE("work3_missing") { CHECK_TRUE(!desk().lookup(77).has_value()); }

TEST_CASE("work3_repo") {
    auto repo = std::make_shared<InMemoryTaskRepository>();
    Task d; d.title = "x"; d.assignee = "y"; d.priority = 2; d.effort = 2;
    auto c = repo->create(d);
    CHECK_TRUE(repo->find_by_id(c.id).has_value());
    c.status = TaskStatus::InProgress;
    CHECK_TRUE(repo->update(c));
    CHECK_TRUE(repo->list().size() == 1);
}

TEST_CASE("work3_order") {
    auto d = desk();
    auto low = d.register_task("L", "a", 5);
    auto high = d.register_task("H", "b", 1);
    auto o = d.board();
    CHECK_EQ(o.front().id, low.id);
    CHECK_EQ(o.back().id, high.id);
}
