#pragma once

#include "Declear.h"
#include "IEventHandler.h"
#include "Buffer.h"
#include "Callbacks.h"

#include <string>
#include <memory>
using namespace std;

namespace nextMuduo {
    class ConnectionHandler : public IEventHandler, public std::enable_shared_from_this<ConnectionHandler>
    {
    public:
        ConnectionHandler(std::shared_ptr<EventLoop> loop, int sockfd);
        ~ConnectionHandler();

    public:
        virtual void handleRead();
        virtual void handleWrite();
        virtual void onWriteComplete();
        void send(const std::string& data);
        void sendInLoop(const string& data);

        /// Set connection callback.
        void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }

        /// Set message callback.
        void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }

        /// Set write complete callback.
        void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
    private:
        int _sockfd;
        std::shared_ptr<Channel> _channel;
        std::shared_ptr<EventLoop> _loop;
        Buffer _inBuffer;
        Buffer _outBuffer;

        ConnectionCallback connectionCallback_;
        MessageCallback messageCallback_;
        WriteCompleteCallback writeCompleteCallback_;
    };
}
