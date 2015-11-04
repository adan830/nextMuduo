#pragma once

#include <iostream>
#include <functional>
using namespace std;

namespace nextMuduo {
class Task
{
public:
	typedef std::function<void()> TaskCB;
public:
	Task(){};
    Task(TaskCB cb);
    void setTask(TaskCB cb);
    void performTask();
private:
    TaskCB _taskCB;
};
}
