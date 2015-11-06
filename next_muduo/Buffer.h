#pragma once

#include <string>
using namespace std;

namespace nextMuduo
{
    class Buffer
    {
    public:
        Buffer();
        ~Buffer();
    public:
        const char* peek();
        int readableBytes();
        void retrieve(int len);
        void append(const string& data);
        std::string retrieveAllAsString();
        std::string retrieveAsString(size_t len);
    private:
        std::string _buf;
    };
}