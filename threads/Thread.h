#pragma once

#include "Task.h"
#include <thread>
using namespace std;

namespace nextMuduo {
class Thread
{
    public:
        Thread(std::shared_ptr<Task> task);
        Thread(Task::TaskCB taskCB);
        ~Thread();
    public:
        void start();
        void join();
    private:
        std::shared_ptr<Task> _task;
        std::shared_ptr<std::thread> _thread;
};
}
