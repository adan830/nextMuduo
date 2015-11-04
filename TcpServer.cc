#include "TcpServer.h"

#include "ConnectionHandler.h"
#include "AcceptorHandler.h"

#include "Log.h"

namespace nextMuduo {

TcpServer::TcpServer(std::shared_ptr<EventLoop> loop)
    :_loop(loop)
{

}

TcpServer::~TcpServer()
{
	LOG_DEBUG << std::endl;
}

void TcpServer::start()
{
    _acceptor.reset(new AcceptorHandler(_loop));
    _acceptor->seTNewConnCB(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
    _acceptor->start();
}

void TcpServer::newConnection(int sockfd)
{
    std::shared_ptr<ConnectionHandler> conn(new ConnectionHandler(_loop, sockfd));
    _connections[sockfd] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    connectionCallback_(conn);
}

}
