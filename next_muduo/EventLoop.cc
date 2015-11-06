#include "next_muduo/EventLoop.h"

#include "next_muduo/Channel.h"
#include "next_muduo/Epoll.h"
#include "next_muduo/threads/CurrentThread.h"
#include "next_muduo/threads/Task.h"

#include <sys/eventfd.h>
#include <unistd.h>
#include <vector>
#include <iostream>
using namespace std;


namespace nextMuduo
{

EventLoop::EventLoop()
    :_quit(false)
    ,_threadId(CurrentThread::tid())
    ,_callingPendingFunctors(false)
    ,_eventfdChannel(std::shared_ptr<Channel>())
{
    _poller.reset(new Epoll());
}

EventLoop::~EventLoop()
{
    LOG_CURRENT_LINE
}

void EventLoop::loop()
{
    if(!_eventfdChannel) {
        _eventfd = createEventfd();
        _eventfdChannel.reset(new Channel(shared_from_this(), _eventfd));
        _eventfdChannel->setEventHandler(this);
        _eventfdChannel->enableReading();
    }

    int loopIndex = 0;
    while (!_quit) {
        LOG_DEBUG << "Loop Cycle(" << loopIndex << ") Start.-----------------------------------<" << std::endl;

        std::vector<Channel*> channels;
        _poller->poll(channels);

        for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
            (*it)->handleEvent();
        }

        doPendingFunctors();
        LOG_DEBUG << "Loop Cycle(" << loopIndex << ") End.------------------------------------->" << std::endl;
        loopIndex++;
    }
}

void EventLoop::update(std::shared_ptr<Channel> channel)
{
	_poller->update(channel);
}

void EventLoop::handleRead()
{
    LOG_CURRENT_LINE
    uint64_t one = 1;
    int n = ::read(_eventfd, &one, sizeof(uint64_t));
    if (n != sizeof(uint64_t)) {
        LOG_ERROR << "reads " << n << " bytes instead of 8" << endl;
    }
}

void EventLoop::handleWrite()
{
    
}

int EventLoop::createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (evtfd < 0) {
        LOG_ERROR << "Failed in eventfd" << std::endl;
    }
    return evtfd;
}

void EventLoop::queueInLoop(Task& task)
{
    {
        MutexLockGuard guard(_mutex);
        _pendingFunctors.push_back(task);
    }
    if(!isInLoopThread() || _callingPendingFunctors)
    {
        wakeup();
    }
}

void EventLoop::runInLoop(Task& task)
{
    LOG_CURRENT_LINE
    if(isInLoopThread())
    {
        task.performTask();
    }
    else
    {
        LOG_DEBUG << "queueInLoop" << std::endl;
        queueInLoop(task);
    }
}

void EventLoop::wakeup()
{
    LOG_CURRENT_LINE
    uint64_t one = 1;
    int n = ::write(_eventfd, &one, sizeof(uint64_t));
    if (n != sizeof(uint64_t)) {
        LOG_ERROR << "writes " << n << " bytes instead of 8" << endl;
    }
}

void EventLoop::doPendingFunctors()
{
    LOG_CURRENT_LINE
    vector<Task> tempRuns;
    _callingPendingFunctors = true;
    {
        MutexLockGuard guard(_mutex);
        tempRuns.swap(_pendingFunctors);
    }
    for(vector<Task>::iterator it = tempRuns.begin(); it != tempRuns.end(); ++it)
    {
        it->performTask();
    }
    _callingPendingFunctors = false;
}

bool EventLoop::isInLoopThread()
{
    return _threadId == CurrentThread::tid();
}

}
