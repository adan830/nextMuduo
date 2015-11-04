#pragma once
namespace nextMuduo {
    class IEventHandler
    {
    public:
        virtual void handleRead() = 0;
        virtual void handleWrite() {};
    };
}
