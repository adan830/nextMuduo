#include "Channel.h"
#include "EventLoop.h"

#include <sys/epoll.h>

#include <assert.h>
#include <iostream>
using namespace std;

namespace nextMuduo {
Channel::Channel(std::shared_ptr<EventLoop> loop, int sockfd)
    :_sockfd(sockfd)
    ,_events(0)
    ,_revents(0)
    ,_index(-1)
    ,_eventHandler(NULL)
    ,_loop(loop)
{

}

Channel::~Channel()
{

}

void Channel::handleEvent()
{
    assert(_eventHandler != NULL);
    if (_revents & EPOLLIN) {
        _eventHandler->handleRead();
    }
    if (_revents & EPOLLOUT) {
        _eventHandler->handleWrite();
    }
}

void Channel::setRevents(int revents)
{
    _revents = revents;
}

void Channel::enableReading()
{
    _events |= EPOLLIN;
    update();
}

void Channel::enableWriting()
{
    _events |= EPOLLOUT;
    update();
}

void Channel::disableWriting()
{
    _events &= ~EPOLLOUT;
    update();
}

bool Channel::isWriting()
{
    return _events & EPOLLOUT;
}

void Channel::setEventHandler(IEventHandler * eventHandler)
{
    _eventHandler = eventHandler;
}

void Channel::update()
{
    assert(_loop);
    _loop->update(shared_from_this());
}

int Channel::getEvents()
{
    return _events;
}

int Channel::getfd()
{
    return _sockfd;
}

int Channel::getIndex()
{
    return _index;
}


void Channel::setIndex(int index)
{
    _index = index;
}

}

