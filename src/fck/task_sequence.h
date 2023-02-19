#ifndef TASKSEQUENCE_CGQHJSRTWYFM_H
#define TASKSEQUENCE_CGQHJSRTWYFM_H

#include "../sigslot/signal.hpp"
#include "common.h"
#include "event_handler.h"

#include <functional>
#include <vector>

namespace fck
{

class TaskSequence : public EventHandler
{
public:
    TaskSequence(const std::string &name = std::string{});
    ~TaskSequence() = default;

    const std::vector<std::function<void()>> &getTasks() const;
    void setTasks(const std::vector<std::function<void()>> &tasks);

    void start();
    void pause();
    void stop();

    void event(Event *);

public:
    sigslot::signal<> task_finished;
    sigslot::signal<> sequence_finished;

private:
    std::vector<std::function<void()>> m_tasks;
    int32_t m_current_task;
    bool m_running;
    bool m_in_process;
};

} // namespace fck

#endif // TASKSEQUENCE_CGQHJSRTWYFM_H
