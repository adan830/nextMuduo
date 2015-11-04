#pragma once

#include "Condition.h"
#include "Mutex.h"

#include <deque>
using namespace std;
namespace nextMuduo {
template<class T>
class BlockingQueue
{
public:
    BlockingQueue()
        :_cond(_mutex)
    {}
    void put(const T& one)
    {
        MutexLockGuard lock(_mutex);
        _queue.push_back(one);
        _cond.notify();
    }

    T take()
    {
        MutexLockGuard lock(_mutex);
        while(_queue.empty())
        {
            _cond.wait();    
        }
        T front(_queue.front());
        _queue.pop_front();
        return front;
    }
private:
    deque<T> _queue;
    MutexLock _mutex;
    Condition _cond;
};
}
