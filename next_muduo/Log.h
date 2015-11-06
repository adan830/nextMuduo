#pragma once

#include "next_muduo/Define.h"
#include "next_muduo/threads/CurrentThread.h"

#include <string>
#include <sstream>
#include <iostream>
#include <ostream>
using namespace std;


namespace nextMuduo {

enum Log_Level { LOG_LEVEL_ERROR = 0, LOG_LEVEL_WARN, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_TRACE };

#define Logging(level) \
        if(LOG_ENABLE || level <= LOG_LEVEL_INFO) (Log::Instance() << Log::LogLevel2String(level) << CurrentThread::TID() << __DATE__ << " " << __TIME__ << " | " \
                   << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): ")

#define LOG_ERROR Logging(LOG_LEVEL_ERROR)
#define LOG_WARN Logging(LOG_LEVEL_WARN)
#define LOG_INFO Logging(LOG_LEVEL_INFO)
#define LOG_DEBUG Logging(LOG_LEVEL_DEBUG)
#define LOG_TRACE Logging(LOG_LEVEL_TRACE)
#define LOG_CURRENT_LINE LOG_DEBUG << std::endl;

class Log
{
public:
    Log();
    ~Log();

public:
    template <typename T> Log& operator<<(const T& value) {
        m_stringBuffer << value;
        return (*this);
    }
    Log& operator<<(ostream& (*_Pfn)(ostream&));
    void flush();
    static Log& Instance();
    static std::string LogLevel2String(Log_Level level);
private:
    std::ostringstream m_stringBuffer;
    static Log * m_instance;
};
}
