#include "Task.h"

namespace nextMuduo {
Task::Task(TaskCB cb)
    :_taskCB(cb)
{
}

void Task::setTask(TaskCB cb)
{
    _taskCB = cb;
}

void Task::performTask()
{
    if(_taskCB) {
        _taskCB();
    }
}
}
