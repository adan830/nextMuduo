#pragma once

#include "Define.h"
#include "threads/CurrentThread.h"

#include <string>
#include <sstream>
#include <iostream>
#include <ostream>
using namespace std;

#define LOG_ERROR if(LOG_ENABLE)(Log::Instance() << "EEEOR " \
                   << CurrentThread::TID() << __DATE__ << __TIME__ << " | " \
                   << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): ")

#define LOG_WARN if(LOG_ENABLE)(Log::Instance() << "WARN " \
                   << CurrentThread::TID() << __DATE__ << __TIME__ << " | " \
                   << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): ")

#define LOG_INFO  if(LOG_ENABLE)(Log::Instance() << "INFO " \
                   << CurrentThread::TID() << __DATE__ << __TIME__ << " | " \
                   << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): ")

#define LOG_DEBUG if(LOG_ENABLE && LOG_DEBUG_ENABLE)(Log::Instance() << "DEBUG "\
                   << CurrentThread::TID() << __DATE__ << __TIME__ << " | " \
                   << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): ")

#define LOG_TRACE if(LOG_ENABLE)(Log::Instance() << "TRACE "\
                   << CurrentThread::TID() << __DATE__ << __TIME__ << " | " \
                   << __FILE__ << ":" << __LINE__ << "(" << __FUNCTION__ << "): ")


namespace nextMuduo {
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
private:
    std::ostringstream m_stringBuffer;
    static Log * m_instance;
};
}
