#pragma once

#include "next_muduo/Declear.h"
#include "next_muduo/IEventHandler.h"

#include <memory>
using namespace std;

namespace nextMuduo {

class Channel : public std::enable_shared_from_this<Channel>
{
public:
    Channel(std::shared_ptr<EventLoop> loop, int sockfd);
    ~Channel();
public:
    void handleEvent();
    void setRevents(int revents);
    void enableReading();
    void enableWriting();
    void disableWriting();
    bool isWriting();
    void setEventHandler(IEventHandler * eventHandler);
    int getfd();
    int getEvents();
    int getIndex();
    void setIndex(int index);
private:
    void update();
private:
    int _sockfd;
    int _events;
    int _revents;
    int _index;
    IEventHandler* _eventHandler;
    std::shared_ptr<EventLoop> _loop;
};
}
