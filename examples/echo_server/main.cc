#include "EchoServer.h"
#include "next_muduo/EventLoop.h"
#include "next_muduo/Log.h"
#include "next_muduo/threads/Thread.h"

#include <iostream>
#include <functional>
using namespace std;

using namespace nextMuduo;

int main(int args, char** argv)
{
    LOG_INFO << "Starting, next-muduo" << std::endl;
    
    {
        // Start echo server
        std::shared_ptr<EventLoop> loop(new EventLoop());
        std::shared_ptr<EchoServer> server(new EchoServer(loop));
        server->start();
        //loop->loop();

        // Start loop
        shared_ptr<Thread> thread;
        thread.reset(new Thread(std::bind(&EventLoop::loop, loop.get())));
        thread->start();

        // Quit after exit_flag assigned.
        int exit_flag;
        std::cin >> exit_flag;
        loop->abort();
    }
    
    LOG_INFO << "Goodbye, next-muduo" << std::endl;
    return 0;
}
