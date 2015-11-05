#pragma once

#include "next_muduo/Declear.h"
#include "next_muduo/IEventHandler.h"

#include <memory>
#include <functional>
using namespace std;

namespace nextMuduo {
class AcceptorHandler : public IEventHandler
{
    typedef std::function<void (int )> TNewConnCB;
public:
    AcceptorHandler(std::shared_ptr<EventLoop> loop);
    ~AcceptorHandler();
public:
    void virtual handleRead();
    void seTNewConnCB(TNewConnCB newConnCB) { m_newConnCB = newConnCB; };
    void start();
private:
    int createAndListen();
private:
    int _listenfd;
    std::shared_ptr<Channel> _acceptChannel;
    TNewConnCB m_newConnCB;
    std::shared_ptr<EventLoop> _loop;
};
}
