#pragma once

#include "Thread.h"
#include "BlockingQueue.h"
#include "Define.h"

#include <vector>
using namespace std;

namespace nextMuduo {
class ThreadPool
{

public:
    ThreadPool(int maxThreadPoolSize = MAX_THREAD_POOL_SIZE);
    ~ThreadPool();

public:
    void start(int numThreads = 1);
    void appendThreads(int numThreads = 0);
    int getThreadNum() { return _threads.size(); }

    void addTask(Task& task);

    void abort();
    bool isAborted() { return _aborted; }

private:
    void runInThread();
    void addAbortTask();

private:
    BlockingQueue<Task> _tasks;
    std::vector<std::shared_ptr<Thread>> _threads;
    MutexLock _mutex;
    bool _aborted;
    int _maxThreadPoolSize;
};
}
