#include "Log.h"
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

Log& Log::Instance() {
    if(m_instance == NULL) {
        m_instance = new Log();
    }
    return (*m_instance);
}

}
