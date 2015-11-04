#pragma once

#include "Declear.h"
#include "IEventHandler.h"
#include "Log.h"
#include "threads/Mutex.h"

#include <vector>
#include <memory>
using namespace std;

namespace nextMuduo
{
    class EventLoop : public IEventHandler, public std::enable_shared_from_this<EventLoop>
    {
    public:
        EventLoop();
        ~EventLoop();
    public:
        void loop();
        void update(std::shared_ptr<Channel> channel);
        virtual void handleRead();
        virtual void handleWrite();
        void queueInLoop(Task& task);
        void runInLoop(Task& task);
        bool isInLoopThread();
        void abort() { _quit = true; wakeup(); }
    private:
        int createEventfd();
        void wakeup();
        void doPendingFunctors();
        
    private:
        bool _quit;
        std::shared_ptr<Epoll> _poller;
        int _eventfd;
        const pid_t _threadId;
        bool _callingPendingFunctors;
        std::shared_ptr<Channel> _eventfdChannel;
        std::vector<Task> _pendingFunctors;
        MutexLock _mutex;
    };
}
