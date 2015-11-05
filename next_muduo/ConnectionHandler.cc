#include "ConnectionHandler.h"

#include "next_muduo/Channel.h"
#include "next_muduo/Define.h"
#include "next_muduo/EventLoop.h"
#include "next_muduo/threads/Task.h"
#include "next_muduo/threads/CurrentThread.h"

#include <errno.h>
#include <unistd.h>
#include <string.h> //for bzero
#include <iostream>
using namespace std;

namespace nextMuduo
{
ConnectionHandler::ConnectionHandler(std::shared_ptr<EventLoop> loop, int sockfd)
    :_sockfd(sockfd)
    ,_loop(loop)
{
    _channel.reset(new Channel(_loop, _sockfd));
    _channel->setEventHandler(this);
    _channel->enableReading();
}

ConnectionHandler::~ConnectionHandler()
{
    LOG_DEBUG << std::endl;
}

void ConnectionHandler::handleRead()
{
    LOG_DEBUG << std::endl;
    int sockfd = _channel->getfd();
    int readlength;
    char line[MAX_READ_BUFFER_SIZE];
    if (sockfd < 0) {
        LOG_DEBUG << "EPOLLIN sockfd < 0 error" << endl;
        return;
    }
    bzero(line, MAX_READ_BUFFER_SIZE);
    readlength = read(sockfd, line, MAX_READ_BUFFER_SIZE);
    if(readlength < 0) {
        if(errno == ECONNRESET) {
            LOG_ERROR << "ECONNREST closed socket fd:" << sockfd << endl;
            close(sockfd);
        }
    }
    else if ( 0 == readlength) {
        LOG_DEBUG << "read 0 from closed socket fd:" << sockfd << endl;
        close(sockfd);
    }
    else {
        LOG_DEBUG << "Message from " << sockfd << ": " << line << endl;
        string linestr(line, readlength);
        _inBuffer.append(linestr);
        if(messageCallback_) messageCallback_(shared_from_this(), _inBuffer);
    }
}

void ConnectionHandler::handleWrite()
{
    LOG_DEBUG << std::endl;
    int sockfd = _channel->getfd();
    if(_channel->isWriting()) {
        int n = ::write(sockfd, _outBuffer.peek(), _outBuffer.readableBytes());
        if( n > 0) {
            LOG_DEBUG << "write " << n << " bytes" <<endl;   
            _outBuffer.retrieve(n);
            if(_outBuffer.readableBytes() == 0) {
                _channel->disableWriting();
                Task task(std::bind(&ConnectionHandler::onWriteComplete, this));
                _loop->queueInLoop(task);
            }
        }
    }
}

void ConnectionHandler::send(const std::string& data)
{
    LOG_DEBUG << std::endl;
    if(_loop->isInLoopThread())
    {
        LOG_DEBUG << "isInLoopThread" << std::endl;
        sendInLoop(data);
    }
    else
    {
        LOG_DEBUG << "runInLoop" << std::endl;
        Task task(std::bind(&ConnectionHandler::sendInLoop, this, data));
        _loop->runInLoop(task);
    }
}

void ConnectionHandler::sendInLoop(const string& data)
{
    LOG_DEBUG << std::endl;
    int n = 0;
    if(_outBuffer.readableBytes() == 0)
    {
        n = ::write(_sockfd, data.c_str(), data.size());
        if(n < 0)
            LOG_DEBUG << "write error" << endl;
        if(n == static_cast<int>(data.size()))
        {
            LOG_DEBUG << "queueInLoop for onWriteComplete" << std::endl;
            Task task(std::bind(&ConnectionHandler::onWriteComplete, this));
            _loop->queueInLoop(task); //invoke onWriteComplate
        }
    }

    if( n < static_cast<int>(data.size()))
    {
        _outBuffer.append(data.substr(n, data.size()));
        if(!_channel->isWriting())
        {
            _channel->enableWriting(); //add EPOLLOUT
        }
    }
}

void ConnectionHandler::onWriteComplete()
{
    if(writeCompleteCallback_) writeCompleteCallback_(shared_from_this());
}

}