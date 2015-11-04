#include "AcceptorHandler.h"

#include "Channel.h"
#include "EventLoop.h"
#include "Define.h"
#include "threads/CurrentThread.h"
#include "Log.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include <assert.h>
#include <iostream>
using namespace std;

namespace nextMuduo {

AcceptorHandler::AcceptorHandler(std::shared_ptr<EventLoop> loop)
    :_listenfd(-1)
    ,_acceptChannel(std::shared_ptr<Channel>())
    ,m_newConnCB(NULL)
    ,_loop(loop)
{

}

AcceptorHandler::~AcceptorHandler()
{
    LOG_DEBUG << std::endl;
}

void AcceptorHandler::start()
{
    _listenfd = createAndListen();
    _acceptChannel.reset(new Channel(_loop, _listenfd));
    _acceptChannel->setEventHandler(this);
    _acceptChannel->enableReading();
}

int AcceptorHandler::createAndListen()
{
    int on = 1;
    _listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    fcntl(_listenfd, F_SETFL, O_NONBLOCK); //no-block io
    setsockopt(_listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(11111);

    if(-1 == bind(_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        LOG_ERROR << "bind error, errno:" << errno << endl;
    }

    if(-1 == listen(_listenfd, MAX_LISTENFD)) {
        LOG_ERROR << "listen error, errno:" << errno << endl;
    }
    return _listenfd;
}

void AcceptorHandler::handleRead()
{
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(struct sockaddr_in);
    int connfd = accept(_listenfd, (sockaddr*)&cliaddr, (socklen_t*)&clilen);
    if (connfd > 0) {
        LOG_DEBUG << "New connection from "
             << "[" << inet_ntoa(cliaddr.sin_addr)
             << ": " << ntohs(cliaddr.sin_port) << "]"
             << " accept sock id:" << connfd << endl;
    } else {
        LOG_DEBUG << "accept error. errno: " << errno << endl;
        return;
    }
    fcntl(connfd, F_SETFL, O_NONBLOCK); //no-block io

    assert(m_newConnCB != NULL);
    m_newConnCB(connfd);
}

}
