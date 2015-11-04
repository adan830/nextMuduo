#pragma once

#include "TcpServer.h"
#include "threads/ThreadPool.h"

using namespace std;

namespace nextMuduo
{

class EchoServer
{
public:
    EchoServer(std::shared_ptr<EventLoop> loop);
    ~EchoServer();
public:
    virtual void onConnection(std::shared_ptr<ConnectionHandler> conn);
    virtual void onMessage(std::shared_ptr<ConnectionHandler> conn, Buffer& buf);
    virtual void onWriteComplete(std::shared_ptr<ConnectionHandler> conn);
    void start();
private:
	void process(const string& str, std::shared_ptr<ConnectionHandler> conn);
private:
    std::shared_ptr<TcpServer> _server;
    ThreadPool _threadpool;
};

}
