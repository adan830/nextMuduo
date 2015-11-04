#pragma once

#include <pthread.h>
#include "Mutex.h"

namespace nextMuduo {
class Condition
{
public:
    Condition(MutexLock& mutex)
        :_mutex(mutex)
    {
        pthread_cond_init(&_condid, NULL);        
    }
    ~Condition()
    {
        pthread_cond_destroy(&_condid); 
    }
    void wait()
    {
        pthread_cond_wait(&_condid, _mutex.getPthreadMutex());
    }
    void notify()
    {
        pthread_cond_signal(&_condid); 
    }
    void notifyAll()
    {
        pthread_cond_broadcast(&_condid);        
    }
private:
    MutexLock& _mutex;
    pthread_cond_t _condid;
};
}
