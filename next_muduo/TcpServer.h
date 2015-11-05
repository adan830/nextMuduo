#pragma once

#include "next_muduo/Declear.h"
#include "next_muduo/Callbacks.h"

#include <map>
#include <memory>
using namespace std;

namespace nextMuduo {

    class TcpServer : public std::enable_shared_from_this<TcpServer>
    {
    public:
        TcpServer(std::shared_ptr<EventLoop> loop);
        ~TcpServer();
    public:
        void start();
        virtual void newConnection(int sockfd);

        /// Set connection callback.
        void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }

        /// Set message callback.
        void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }

        /// Set write complete callback.
        void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
    private:
        map<int, std::shared_ptr<ConnectionHandler>> _connections;
        std::shared_ptr<AcceptorHandler> _acceptor;
        std::shared_ptr<EventLoop> _loop;
        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
    };
}
