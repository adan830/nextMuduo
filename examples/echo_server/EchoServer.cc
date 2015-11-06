#include "EchoServer.h"

#include "next_muduo/ConnectionHandler.h"
#include "next_muduo/TcpServer.h"
#include "next_muduo/EventLoop.h"
#include "next_muduo/threads/CurrentThread.h"
#include "next_muduo/threads/ThreadPool.h"

#include <string>
#include <iostream>
using namespace std;

#define MESSAGE_LENGTH       (256)

namespace nextMuduo
{

EchoServer::EchoServer(std::shared_ptr<EventLoop> eventLoop)
{
    _server.reset(new TcpServer(eventLoop));
    _threadpool.reset(new ThreadPool);
}

EchoServer::~EchoServer()
{
    LOG_CURRENT_LINE
}

void EchoServer::start()
{
    _server->setConnectionCallback(
        std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
    _server->setMessageCallback(
        std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    _server->setWriteCompleteCallback(
        std::bind(&EchoServer::onWriteComplete, this, std::placeholders::_1));
    _server->start();
    _threadpool->start(3);
}

void EchoServer::onConnection(std::shared_ptr<ConnectionHandler> conn)
{
    LOG_CURRENT_LINE
}

void EchoServer::onMessage(std::shared_ptr<ConnectionHandler> conn, Buffer& buf)
{
    LOG_DEBUG << "Length: "<< buf.readableBytes() << ": " << buf.peek() << std::endl;
    if (buf.readableBytes() <= MESSAGE_LENGTH) {
        //conn->send(buf->retrieveAllAsString());
        Task task(std::bind(&EchoServer::process, this, buf.retrieveAllAsString(), conn));
        _threadpool->addTask(task);
    }
    else {
        while (buf.readableBytes() > MESSAGE_LENGTH) {
            string message = buf.retrieveAsString(MESSAGE_LENGTH);
            Task task(std::bind(&EchoServer::process, this, message, conn));
            _threadpool->addTask(task);
        }
    }
}

void EchoServer::onWriteComplete(std::shared_ptr<ConnectionHandler> conn) 
{
    LOG_CURRENT_LINE
}

//fib is short for Fibonacci, fib is a CPU busy method
int fib(int n)
{
    return (n == 1 || n == 2) ? 1 : (fib(n-1) + fib(n-2));
}

void EchoServer::process(const string& str, std::shared_ptr<ConnectionHandler> conn)
{
    LOG_CURRENT_LINE
    //IO blocking task or CPU busy task
    //LOG_DEBUG << "fib(30) = " << fib(30) << " tid = " << CurrentThread::tid() << endl;
    conn->send(str);
}

}
