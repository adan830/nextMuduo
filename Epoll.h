#pragma once

#include <sys/epoll.h>

#include "Declear.h"
#include "Define.h"

#include <vector>
#include <memory>
using namespace std;

namespace nextMuduo
{
    class Epoll
    {
    public:
        Epoll();
        ~Epoll();
    public:
        void poll(std::vector<Channel*>& channels);
        void update(std::shared_ptr<Channel> channel);
    private:
        int _epollfd;
        struct epoll_event _events[MAX_EVENTS];
    };
}
