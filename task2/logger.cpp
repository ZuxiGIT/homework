#include "logger.hpp"
#include <iostream>
#include <cassert>

std::ostringstream Logger::m_output = {};
std::string Logger::filepath = "";
std::string LogObj::lastObj = {};
std::string LogFunc::current_function = "";
int LogFunc::function_counter = 0;
unsigned int Logger::instances = 0;
int Logger::m_shift  = 0;

Logger::Logger(const std::string& filename)
{
    _ERROR("LOGGER(std::string)\n")
    
    instances++;
    filepath = filename;
    
    if(!m_output.good())
    {
        _ERROR("SOMETHING WRONG")
        fflush(0);
        exit(1);
    }
    
    m_output << "digraph test{\n";

    m_shift++;
    shift();
    m_output << "rankdir=TB;\n";
}

Logger::Logger()
{
    instances++;
    _ERROR("LOGGER()\n")
}

void Logger::shift()
{
    for(int i = 0; i < m_shift; ++i)
        m_output << "\t";
}


Logger::~Logger()
{
	_ERROR("~Logger()\n")
    instances--;
    if(!instances)
    {
        m_output << "}";
        FILE* fp = fopen(filepath.c_str(), "w");
        
        if(!fp)
        {
            _ERROR("ERROR::FILE_NOT_OPENED\n")
            return;
        }

        fprintf(fp, m_output.str().c_str());
    }
}



LogFunc::LogFunc(const std::string& funcname)
{
	function_counter++;
	_ERROR("LOGFUNC()\n")
	
    
	shift();
    m_output << "subgraph cluster_" << _CHAR(function_counter) << "{\n"; 
    
    m_shift++;

    shift();
    m_output << "color=lightgrey;\n";
    shift();
    m_output << "style=filled;\n";
    shift();
    m_output << "label= \""<< funcname << "\";\n";

}

LogFunc::~LogFunc()
{
	m_shift--;
	shift();
	m_output << "}\n";

    _ERROR("~LOGFUNC()\n");
}

LogObj::LogObj(Obj2log& obj)
{
	_ERROR("LogObj()\n")	
	std::string Node = obj.createGraphNode();
	
	if(!Node.empty())
	{
		shift();
		m_output << Node;
	}

	std::string edge = obj.createHistoryEdge();
	
	if(!edge.empty())
	{
		shift();
		m_output << edge;
	}

}

LogObj::~LogObj()
{
	_ERROR("~LogObj()\n")
}