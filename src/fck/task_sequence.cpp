#include "task_sequence.h"
#include "event_dispatcher.h"

namespace fck
{

TaskSequence::TaskSequence(const std::string &name)
    : EventHandler{{}, {}, name}, m_current_task{0}, m_running{false}, m_in_process{false}
{
}

const std::vector<std::function<void()>> &TaskSequence::getTasks() const
{
    return m_tasks;
}

void TaskSequence::setTasks(const std::vector<std::function<void()>> &tasks)
{
    stop();
    m_tasks = tasks;
}

void TaskSequence::start()
{
    if (m_running || m_in_process || m_tasks.empty() || m_current_task >= int32_t(m_tasks.size()))
        return;
    m_running = true;
    m_in_process = true;
    EventDispatcher::push(event_type::TASK_SEQUENCE, this);
}

void TaskSequence::pause()
{
    if (!m_running)
        return;
    m_running = false;
}

void TaskSequence::stop()
{
    if (!m_running)
        return;
    m_running = false;
    m_in_process = false;
    m_current_task = 0;
}

void TaskSequence::event(Event *)
{
    if (m_in_process)
    {
        m_tasks[m_current_task]();
        m_in_process = false;
        ++m_current_task;
        if (m_current_task >= int32_t(m_tasks.size()))
        {
            m_running = false;
            m_current_task = 0;
        }
    }

    if (m_running)
    {
        m_in_process = true;
        EventDispatcher::push(event_type::TASK_SEQUENCE, this);
    }
}

} // namespace fck
