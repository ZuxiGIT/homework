#include "logger.hpp"
#include <string.h>
#include <iostream>
#include <cassert>

#define _ERROR(_str) fprintf(stderr, _str);

Logger::Logger(std::string&& filename)
{
    assert(!m_created && "ERROR::FILE_IS_ALREADY_OPENED\n" );
    
    m_output.open(filename, std::ios_base::out | std::ios_base::trunc);
    
    if(!m_output.is_open())
    {
        _ERROR("ERROR::CANNOT_OPEN_FILE\n")
        exit(1);
    }

    m_created = true;
    m_output << "digraph test{\n";
}

Logger::Logger()
{
    assert(m_created && "Output was not set");
    m_shift++;
}

void Logger::shift()
{
    for(int i = 0; i < m_shift; ++i)
        m_output<<"\t";
}

Logger::~Logger()
{
    m_output << "}";
    _ERROR("CLOSING FILE")
    m_output.close();
}



// LogFunc::LogFunc(std::string&& funcname)
// {
//     m_output << "}\n";

//     m_lastFunc = currFunc;
//     m_currFunc = funcname.;
//     m_output <<
// }

LogFunc::~LogFunc()
{
    _ERROR("DESTROYING LOGFUNC()\n");
}