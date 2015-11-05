#include "next_muduo/Log.h"

namespace nextMuduo {

Log * Log::m_instance = NULL;

Log::Log()
{
}

Log::~Log()
{
}

Log& Log::operator<<(ostream& (*_Pfn)(ostream&))
{
    _Pfn(m_stringBuffer);
    flush();
    return (*this);
}

void Log::flush() {
        std::cout << m_stringBuffer.str();
        m_stringBuffer.str("");
}

std::string Log::LogLevel2String(Log_Level level)
{
    switch(level) {
        case LOG_LEVEL_ERROR:
            return "ERROR";
        case LOG_LEVEL_WARN:
            return "WARN";
        case LOG_LEVEL_INFO:
            return "INFO";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
        case LOG_LEVEL_TRACE:
            return "TRACE";
        default:
            return "UNKNOWN";
    }
}

Log& Log::Instance() {
    if(m_instance == NULL) {
        m_instance = new Log();
    }
    return (*m_instance);
}

}
