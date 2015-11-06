#include "next_muduo/Epoll.h"
#include "next_muduo/Channel.h"
#include "next_muduo/Define.h"
#include "next_muduo/Log.h"

#include <errno.h>

#include <iostream>
using namespace std;

namespace nextMuduo
{

static const int kNew = -1;
static const int kAdded = 1;

Epoll::Epoll()
{
	_epollfd = ::epoll_create(1);
    if (_epollfd < 0) {
        LOG_ERROR << "epoll_create error. epollfd: " << _epollfd << endl;
    }
}

Epoll::~Epoll()
{
    LOG_CURRENT_LINE
}

void Epoll::poll(std::vector<Channel*>& channels)
{
	int fds = ::epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
    if (fds == -1) {
        LOG_ERROR << "epoll_wait error. errno: " << errno << endl;
        return;
    }

    for (int i = 0; i < fds; i++) {
        Channel* channel = static_cast<Channel*>(_events[i].data.ptr);
        LOG_DEBUG << "New event handling from " << channel->getfd() << endl;
        channel->setRevents(_events[i].events);
        channels.push_back(channel);
    }
}

void Epoll::update(std::shared_ptr<Channel> channel)
{
    struct epoll_event ev;
    ev.data.ptr = channel.get();
    ev.events = channel->getEvents();
    int sockfd = channel->getfd();

    int index = channel->getIndex();
    if (kNew == index) {
        channel->setIndex(kAdded);
        ::epoll_ctl(_epollfd, EPOLL_CTL_ADD, sockfd, &ev);
        LOG_DEBUG << "Adding sockfd: " << channel->getfd() << " to epollfd: " << _epollfd << endl;
    }
    else {
        ::epoll_ctl(_epollfd, EPOLL_CTL_MOD, sockfd, &ev);
        LOG_DEBUG << "Modify sockfd: " << channel->getfd() << " to epollfd: " << _epollfd << endl;
    }
}

}